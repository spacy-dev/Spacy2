#ifndef ALGORITHMS_ADAPTER_FENICS_FUNCTIONAL_HH
#define ALGORITHMS_ADAPTER_FENICS_FUNCTIONAL_HH

#include <memory>

#include "../../vector.hh"
#include "../../vectorSpace.hh"

#include "Util/Mixins/disableAssembly.hh"
#include "Util/Mixins/primalDualSwitch.hh"
#include "Util/cast.hh"
#include "Util/Base/functionalBase.hh"

#include "assignXIfPresent.hh"
#include "hessian.hh"
#include "linearSolver.hh"
#include "operator.hh"
#include "util.hh"
#include "vector.hh"

namespace Algorithm
{
  namespace Fenics
  {
    /**
     * @ingroup FenicsGroup
     * @brief Functional interface for FEniCS. Models a twice differentiable functional \f$f:X\rightarrow \mathbb{R}\f$.
     * @tparam F dolfin::Form describing the functional value
     * @tparam DF dolfin::Form describing the derivative
     * @tparam DDF dolfin::Form describing the second derivative
     * @see C2Functional, C2FunctionalConcept
     */
    template <class F, class DF, class DDF>
    class Functional :
        public FunctionalBase< Functional<F,DF,DDF> > ,
        public Mixin::DisableAssembly
    {
    public:
      /**
       * @brief Construct functional for FEnics.
       * @param f form for the evaluation of \f$f\f$.
       * @param J form for the evaluation of \f$f'\f$
       * @param J form for the evaluation of \f$f''\f$
       * @param bcs Dirichlet boundary conditions
       * @param space domain space \f$X\f$
       */
      Functional(const F& f, const DF& J, const DDF& H,
                 const std::vector<const dolfin::DirichletBC*>& bcs, ::Algorithm::VectorSpace& space)
        : FunctionalBase< Functional<F,DF,DDF> >( space ),
          f_( f.mesh_shared_ptr() ),
          J_( J.function_space(0) ),
          H_( H.function_space(0) , H.function_space(1) ),
          bcs_( bcs )
      {
        copyCoefficients(f,f_);
        copyCoefficients(J,J_);
        copyCoefficients(H,H_);
      }

      /// Copy constructor.
      Functional(const Functional& other)
        : FunctionalBase< Functional<F,DF,DDF> >( other.domain() ) ,
          Mixin::DisableAssembly( other.assemblyIsDisabled() ) ,
          f_( other.f_.mesh_shared_ptr() ) ,
          J_( other.J_.function_space(0) ) ,
          H_( other.H_.function_space(0) , other.H_.function_space(1) ) ,
          bcs_( other.bcs_ ),
          A_( (other.A_!=nullptr) ? other.A_->copy() : nullptr ) ,
          b_( (other.b_!=nullptr) ? other.b_->copy() : nullptr ) ,
          value_( other.value_ ) ,
          valueAssembled_( other.valueAssembled_ ) ,
          oldX_f_(other.oldX_f_) , oldX_J_(other.oldX_J_) , oldX_H_(other.oldX_H_)
      {
        copyCoefficients(other.f_,f_);
        copyCoefficients(other.J_,J_);
        copyCoefficients(other.H_,H_);
      }

      /// Copy assignment.
      Functional& operator=(const Functional& other)
      {
        disableAssembly( other.assemblyIsDisabled() );
        bcs_ = other.bcs_;
        A_ = (other.A_!=nullptr) ? other.A_->copy() : nullptr;
        b_ = (other.b_!=nullptr) ? other.b_->copy() : nullptr;
        value_ = other.value_;
        valueAssembled_ = other.valueAssembled_;
        oldX_f_ = other.oldX_f_;
        oldX_J_ = other.oldX_J_;
        oldX_H_ = other.oldX_H_;

        copyCoefficients(other.f_,f_);
        copyCoefficients(other.J_,J_);
        copyCoefficients(other.H_,H_);
      }

      /// Move constructor.
      Functional(Functional&& other)
        : FunctionalBase< Functional<F,DF,DDF> >( other.domain() ) ,
          Mixin::DisableAssembly( other.assemblyIsDisabled() ) ,
          f_( other.f_.mesh_shared_ptr() ) ,
          J_( other.J_.function_space(0) ) ,
          H_( other.H_.function_space(0) , other.H_.function_space(1) ) ,
          bcs_( other.bcs_ ),
          A_( std::move(other.A_) ) ,
          b_( std::move(other.b_) ) ,
          value_( other.value_ ) ,
          valueAssembled_( other.valueAssembled_ ) ,
          oldX_f_(std::move(other.oldX_f_)) , oldX_J_(std::move(other.oldX_J_)) , oldX_H_(std::move(other.oldX_H_))
      {
        copyCoefficients(other.f_,f_);
        copyCoefficients(other.J_,J_);
        copyCoefficients(other.H_,H_);
      }

      /// Move assignment.
      Functional& operator=(Functional&& other)
      {
        disableAssembly( other.assemblyIsDisabled() );
        bcs_ = std::move(other.bcs_);
        A_ = std::move(other.A_);
        b_ = std::move(other.b_);
        value_ = other.value_;
        valueAssembled_ = other.valueAssembled_;
        oldX_f_ = std::move(other.oldX_f_);
        oldX_J_ = std::move(other.oldX_J_);
        oldX_H_ = std::move(other.oldX_H_);

        copyCoefficients(other.f_,f_);
        copyCoefficients(other.J_,J_);
        copyCoefficients(other.H_,H_);
      }

      /// Compute functional value \f$f(x)\f$.
      double operator()(const ::Algorithm::Vector& x) const
      {
        primalDualIgnoreReset(std::bind(&Functional::assembleFunctional,std::ref(*this), std::placeholders::_1),x);

        return value_;
      }

      /**
       * @brief Compute first directional derivative \f$f'(x):\ X \rightarrow X^* \f$.
       *
       * Actual implementation of d1 is provided in base class FunctionalBase.
       * @see ::Algorithm::FunctionalBase
       */
      ::Algorithm::Vector d1_(const ::Algorithm::Vector &x) const
      {
        primalDualIgnoreReset(std::bind(&Functional::assembleJacobian,std::ref(*this), std::placeholders::_1),x);

        auto y = this->domain().dualSpace_ptr()->element();
        copy(*b_,y);
        return y;
      }

      /**
       * @brief Compute second directional derivative \f$f''(x):\ X \rightarrow X^* \f$.
       *
       * Actual implementation of d2 is provided in base class FunctionalBase.
       * @see ::Algorithm::FunctionalBase
       */
      ::Algorithm::Vector d2_(const ::Algorithm::Vector &x, const ::Algorithm::Vector &dx) const
      {
        primalDualIgnoreReset(std::bind(&Functional::assembleHessian,std::ref(*this), std::placeholders::_1),x);

        auto x_ = dolfin::Function( J_.function_space(0) );
        copy(dx,x_);
        auto Ax = x_.vector()->copy();
        A_->mult(*x_.vector(), *Ax);

        auto result = this->domain().dualSpace_ptr()->element();
        copy(*Ax,result);

        return result;
      }

      /**
       * @brief Access \f$f''(x)\f$ as linear operator \f$X\rightarrow X^*\f$.
       * @see Hessian, LinearOperator, LinearOperatorConcept
       */
      LinearOperator hessian(const ::Algorithm::Vector& x) const
      {
        primalDualIgnoreReset(std::bind(&Functional::assembleHessian,std::ref(*this), std::placeholders::_1),x);

        assert( A_ != nullptr );
        auto newFunctional = *this;
        newFunctional.disableAssembly();
        return Hessian(std::move(newFunctional), x, LUSolver(*A_,*J_.function_space(0),this->domain().dualSpace(),this->domain(),true) );
      }

    private:
      /// Assemble functional value \f$f(x)\f$.
      void assembleFunctional(const ::Algorithm::Vector& x) const
      {
        if( valueAssembled_ && (oldX_f_ == x) ) return;
        valueAssembled_ = true;
        auto x_ = dolfin::Function( J_.function_space(0) );
        copy(x,x_);

        Assign_X_If_Present<decltype(f_)>::apply(f_,x_);
        value_ = dolfin::assemble(f_);

        oldX_f_ = x;
      }

      /// Assemble discrete representation of \f$f'(x)\f$.
      void assembleJacobian(const ::Algorithm::Vector& x) const
      {
        if( assemblyIsDisabled() ) return;
        if( b_ != nullptr && (oldX_J_==x) ) return;

        auto x_ = dolfin::Function( J_.function_space(0) );
        copy(x,x_);
        Assign_X_If_Present<decltype(J_)>::apply(J_,x_);
        b_ = x_.vector()->factory().create_vector();

        dolfin::assemble(*b_,J_);
        for( auto& bc : bcs_) bc->apply(*b_,*x_.vector());

        oldX_J_ = x;
      }

      /// Assemble discrete representation of \f$f''(x)\f$.
      void assembleHessian(const ::Algorithm::Vector& x) const
      {
        if( assemblyIsDisabled() ) return;

        if( A_ != nullptr && (oldX_H_==x) ) return;

        auto x_ = dolfin::Function( J_.function_space(0) );
        copy(x,*x_.vector());
        Assign_X_If_Present<decltype(H_)>::apply(H_,x_);
        A_ = x_.vector()->factory().create_matrix();
        dolfin::assemble(*A_,H_);

        for( auto& bc : bcs_) bc->apply(*A_,*x_.vector(),*x_.vector());

        oldX_H_ = x;
      }

      mutable F f_;
      mutable DF J_;
      mutable DDF H_;
      const std::vector<const dolfin::DirichletBC*>& bcs_;
      mutable std::shared_ptr<dolfin::GenericMatrix> A_;
      mutable std::shared_ptr<dolfin::GenericVector> b_;
      mutable double value_ = 0;
      mutable bool valueAssembled_ = false;
      mutable ::Algorithm::Vector oldX_f_, oldX_J_, oldX_H_;
    };

    /**
     * @ingroup FenicsGroup
     * @brief Convenient generation of a twice differentiable functional \f$f: X\rightarrow \mathbb{R}\f$ as used in FEniCS.
     * @return ::Algorithm::Fenics::Functional<F,DF,DDF>( f , J , H , bcs , space )
     */
    template <class F, class DF, class DDF>
    auto makeFunctional( const F& f , const DF& J , const DDF& H ,
                         const std::vector<const dolfin::DirichletBC*>& bcs, VectorSpace& space)
    {
      return Fenics::Functional<F,DF,DDF>( f , J , H , bcs , space );
    }

    /**
     * @ingroup FenicsGroup
     * @brief Convenient generation of a twice differentiable functional \f$f: X\rightarrow \mathbb{R}\f$
     * with out Dirichlet boundary conditions as used in FEniCS.
     * @return ::Algorithm::Fenics::Functional<F,DF,DDF>( f , J , H , bcs , space )
     */
    template <class F, class DF, class DDF>
    auto makeFunctional( const F& f , const DF& J , const DDF& H , VectorSpace& space)
    {
      return Fenics::Functional<F,DF,DDF>( f , J , H , {} , space );
    }
  }
}

#endif // ALGORITHMS_ADAPTER_FENICS_FUNCTIONAL_HH

