// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACYS_ADAPTER_FENICS_C2_FUNCTIONAL_HH
#define SPACYS_ADAPTER_FENICS_C2_FUNCTIONAL_HH

#include <memory>

#include "Spacy/vector.hh"
#include "Spacy/vectorSpace.hh"

#include "Spacy/Util/cast.hh"
#include "Spacy/Util/Base/functionalBase.hh"

#include "assignXIfPresent.hh"
#include "util.hh"
#include "vector.hh"
#include "operatorSpace.hh"
#include "linearOperator.hh"
#include "Spacy/operator.hh"

namespace Spacy
{
  /** @addtogroup FenicsGroup
   * @{
   */
  namespace FEniCS
  {
    /**
     * @brief %Functional interface for %FEniCS. Models a twice differentiable functional \f$f:X\rightarrow \mathbb{R}\f$.
     * @tparam F dolfin::Form describing the functional value
     * @tparam DF dolfin::Form describing the derivative
     * @tparam DDF dolfin::Form describing the second derivative
     * @warning In the .ufl file you have to name the argument of \f$f\f$ by "x"!
     * @see Spacy::C2Functional
     */
    template <class F, class DF, class DDF>
    class C2Functional : public FunctionalBase
    {
    public:
      /**
       * @brief Construct functional for FEnics.
       * @param f form for the evaluation of \f$f\f$.
       * @param J form for the evaluation of \f$f'\f$
       * @param H form for the evaluation of \f$f''\f$
       * @param bcs Dirichlet boundary conditions
       * @param space domain space \f$X\f$
       */
      C2Functional(const F& f, const DF& J, const DDF& H,
                 const std::vector<const dolfin::DirichletBC*>& bcs, const VectorSpace& domain)
        : FunctionalBase( domain ),
          f_( f.mesh_shared_ptr() ),
          J_( J.function_space(0) ),
          H_( H.function_space(0) , H.function_space(1) ),
          bcs_( bcs ) ,
          operatorSpace_( std::make_shared<VectorSpace>( LinearOperatorCreator(domain,domain.dualSpace(),J.function_space(0)) , [](const Spacy::Vector& v)
          {
            return dolfin::Matrix( cast_ref<LinearOperator>(v).impl()).norm("frobenius");
          } , true ) )
      {
        copyCoefficients(f,f_);
        copyCoefficients(J,J_);
        copyCoefficients(H,H_);
      }

      /**
       * @brief Copy constructor.
       * @param g functional to copy from
       */
      C2Functional(const C2Functional& g)
        : FunctionalBase( g.domain() ) ,
          f_( g.f_.mesh_shared_ptr() ) ,
          J_( g.J_.function_space(0) ) ,
          H_( g.H_.function_space(0) , g.H_.function_space(1) ) ,
          bcs_( g.bcs_ ),
          A_( (g.A_!=nullptr) ? g.A_->copy() : nullptr ) ,
          b_( (g.b_!=nullptr) ? g.b_->copy() : nullptr ) ,
          value_( g.value_ ) ,
          valueAssembled_( g.valueAssembled_ ) ,
          oldX_f_(g.oldX_f_) , oldX_J_(g.oldX_J_) , oldX_H_(g.oldX_H_) ,
          operatorSpace_(g.operatorSpace_)
      {
        copyCoefficients(g.f_,f_);
        copyCoefficients(g.J_,J_);
        copyCoefficients(g.H_,H_);
      }

      /**
       * @brief Copy assignment.
       * @param g functional to copy from
       */
      C2Functional& operator=(const C2Functional& g)
      {
        bcs_ = g.bcs_;
        A_ = (g.A_!=nullptr) ? g.A_->copy() : nullptr;
        b_ = (g.b_!=nullptr) ? g.b_->copy() : nullptr;
        value_ = g.value_;
        valueAssembled_ = g.valueAssembled_;
        oldX_f_ = g.oldX_f_;
        oldX_J_ = g.oldX_J_;
        oldX_H_ = g.oldX_H_;
        operatorSpace_ = g.operatorSpace_;

        copyCoefficients(g.f_,f_);
        copyCoefficients(g.J_,J_);
        copyCoefficients(g.H_,H_);
      }

      /**
       * @brief Move constructor.
       * @param g functional to move from
       */
      C2Functional(C2Functional&& g)
        : FunctionalBase( g.domain() ) ,
          f_( g.f_.mesh_shared_ptr() ) ,
          J_( g.J_.function_space(0) ) ,
          H_( g.H_.function_space(0) , g.H_.function_space(1) ) ,
          bcs_( g.bcs_ ),
          A_( std::move(g.A_) ) ,
          b_( std::move(g.b_) ) ,
          value_( g.value_ ) ,
          valueAssembled_( g.valueAssembled_ ) ,
          oldX_f_(std::move(g.oldX_f_)) , oldX_J_(std::move(g.oldX_J_)) , oldX_H_(std::move(g.oldX_H_)),
          operatorSpace_(std::move(g.operatorSpace_))
      {
        copyCoefficients(g.f_,f_);
        copyCoefficients(g.J_,J_);
        copyCoefficients(g.H_,H_);
      }

      /**
       * @brief Move assignment.
       * @param g functional to move from
       */
      C2Functional& operator=(C2Functional&& g)
      {
        bcs_ = std::move(g.bcs_);
        A_ = std::move(g.A_);
        b_ = std::move(g.b_);
        value_ = g.value_;
        valueAssembled_ = g.valueAssembled_;
        oldX_f_ = std::move(g.oldX_f_);
        oldX_J_ = std::move(g.oldX_J_);
        oldX_H_ = std::move(g.oldX_H_);
        operatorSpace_ = std::move(g.operatorSpace_);

        copyCoefficients(g.f_,f_);
        copyCoefficients(g.J_,J_);
        copyCoefficients(g.H_,H_);
      }

      /**
       * @brief Apply functional.
       * @param x argument
       * @return \f$f(x)\f$
       */
      Real operator()(const ::Spacy::Vector& x) const
      {
        assembleFunctional(x);
        return value_;
      }

      /**
       * @brief Compute first directional derivative \f$f'(x) \in X^* \f$.
       *
       * @param x current iterate
       * @return \f$f'(x)\f$
       */
      ::Spacy::Vector d1(const ::Spacy::Vector &x) const
      {
        assembleJacobian(x);

        auto y = this->domain().dualSpace().zeroVector();
        copy(*b_,y);
        return y;
      }

      /**
       * @brief Compute second directional derivative \f$f''(x)dx\in X^* \f$.
       *
       * @param x current iterate
       * @param dx perturbation
       * @return \f$f''(x)dx\f$
       */
      ::Spacy::Vector d2(const ::Spacy::Vector &x, const ::Spacy::Vector &dx) const
      {
        assembleHessian(x);

        auto x_ = dolfin::Function( J_.function_space(0) );
        copy(dx,x_);
        auto Ax = x_.vector()->copy();
        A_->mult(*x_.vector(), *Ax);

        auto result = this->domain().dualSpace().zeroVector();
        copy(*Ax,result);

        return result;
      }

      /**
       * @brief Access \f$f''(x)\f$ as linear operator \f$X\rightarrow X^*\f$.
       * @param x point of linearization
       * @see Hessian, @ref LinearOperatorAnchor "LinearOperator", @ref LinearOperatorConceptAnchor "LinearOperatorConcept"
       */
      ::Spacy::LinearOperator hessian(const ::Spacy::Vector& x) const
      {
        assembleHessian(x);

        assert( A_ != nullptr );
        return LinearOperator{ A_->copy() , *operatorSpace_ , J_.function_space(0) };
//        return Hessian(*this, x, LUSolver(*A_,this->domain().dualSpace(),this->domain(),true) );
      }

    private:
      /// Assemble functional value \f$f(x)\f$.
      void assembleFunctional(const ::Spacy::Vector& x) const
      {
        if( valueAssembled_ && (oldX_f_ == x) ) return;
        valueAssembled_ = true;
        auto x_ = dolfin::Function( J_.function_space(0) );
        copy(x,x_);

        assign_x_if_present(f_,x_);
        value_ = dolfin::assemble(f_);

        oldX_f_ = x;
      }

      /// Assemble discrete representation of \f$f'(x)\f$.
      void assembleJacobian(const ::Spacy::Vector& x) const
      {
        if( b_ != nullptr && (oldX_J_==x) ) return;

        auto x_ = dolfin::Function( J_.function_space(0) );
        copy(x,x_);
        assign_x_if_present(J_,x_);
        b_ = x_.vector()->factory().create_vector();

        dolfin::assemble(*b_,J_);
        for( auto& bc : bcs_) bc->apply(*b_,*x_.vector());

        oldX_J_ = x;
      }

      /// Assemble discrete representation of \f$f''(x)\f$.
      void assembleHessian(const ::Spacy::Vector& x) const
      {
        if( A_ != nullptr && (oldX_H_==x) ) return;

        auto x_ = dolfin::Function( J_.function_space(0) );
        copy(x,*x_.vector());
        assign_x_if_present(H_,x_);
        A_ = x_.vector()->factory().create_matrix();
        dolfin::assemble(*A_,H_);

        for( auto& bc : bcs_) bc->apply(*A_,*x_.vector(),*x_.vector());

        oldX_H_ = x;
      }

      mutable F f_;
      mutable DF J_;
      mutable DDF H_;
      std::vector<const dolfin::DirichletBC*> bcs_ = {};
      mutable std::shared_ptr<dolfin::GenericMatrix> A_ = nullptr;
      mutable std::shared_ptr<dolfin::GenericVector> b_ = nullptr;
      mutable double value_ = 0;
      mutable bool valueAssembled_ = false;
      mutable ::Spacy::Vector oldX_f_ = {}, oldX_J_ = {}, oldX_H_ = {};
      std::shared_ptr<VectorSpace> operatorSpace_ = nullptr;
    };

    /**
     * @brief Convenient generation of a twice differentiable functional \f$f: X\rightarrow \mathbb{R}\f$ as used in %FEniCS.
     * @param f form for the evaluation of \f$f\f$.
     * @param J form for the evaluation of \f$f'\f$
     * @param H form for the evaluation of \f$f''\f$
     * @param bcs Dirichlet boundary conditions
     * @param space domain space \f$X\f$
     * @return @ref C2Functional "::Spacy::Fenics::C2Functional<F,DF,DDF>( f , J , H , bcs , space )"
     */
    template <class F, class DF, class DDF>
    auto makeC2Functional( const F& f , const DF& J , const DDF& H ,
                         const std::vector<const dolfin::DirichletBC*>& bcs, const VectorSpace& space)
    {
      return C2Functional<F,DF,DDF>( f , J , H , bcs , space );
    }

    /**
     * @brief Convenient generation of a twice differentiable functional \f$f: X\rightarrow \mathbb{R}\f$
     * with out Dirichlet boundary conditions as used in %FEniCS.
     * @param f form for the evaluation of \f$f\f$.
     * @param J form for the evaluation of \f$f'\f$
     * @param H form for the evaluation of \f$f''\f$
     * @param space domain space \f$X\f$
     * @return @ref C2Functional "::Spacy::Fenics::C2Functional<F,DF,DDF>( f , J , H , bcs , space )"
     */
    template <class F, class DF, class DDF>
    auto makeC2Functional( const F& f , const DF& J , const DDF& H , const VectorSpace& space)
    {
      return C2Functional<F,DF,DDF>( f , J , H , {} , space );
    }
  }
  /** @} */
}

#endif // SPACYS_ADAPTER_FENICS_C2_FUNCTIONAL_HH

