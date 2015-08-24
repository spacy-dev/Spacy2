#ifndef ALGORITHMS_ADAPTER_FENICS_FUNCTIONAL_HH
#define ALGORITHMS_ADAPTER_FENICS_FUNCTIONAL_HH

#include <memory>

#include "linearSolver.hh"
#include "hessian.hh"
#include "operator.hh"
#include "vector.hh"
#include "Util/Mixins/disableAssembly.hh"
#include "Util/Mixins/primalDualSwitch.hh"
#include "Util/cast.hh"
#include "Util/Base/functionalBase.hh"

#include "../../vector.hh"
#include "../../vectorSpace.hh"

#include "util.hh"
#include "assignXIfPresent.hh"

namespace Algorithm
{
  namespace Fenics
  {
    template <class F, class DF, class DDF>
    class Functional :
        public FunctionalBase< Functional<F,DF,DDF> > ,
        public Mixin::DisableAssembly
    {
    public:
      Functional(const F& f, const DF& J, const DDF& H,
                 const std::vector<const dolfin::DirichletBC*>& bcs, ::Algorithm::VectorSpace* space)
        : FunctionalBase< Functional<F,DF,DDF> >( space ),
          f_( f.mesh_shared_ptr() ),
          J_( J.function_space(0) ),
          H_( H.function_space(0) , H.function_space(1) ),
          bcs_( bcs ),
          dummy_(J_.function_space(0))
      {
        copyCoefficients(f,f_);
        copyCoefficients(J,J_);
        copyCoefficients(H,H_);
      }

      Functional(const F& f, const DF& J, const DDF& H,
                 const std::vector<const dolfin::DirichletBC*>& bcs, ::Algorithm::VectorSpace& space)
        : Functional(f,J,H,bcs,&space)
      {}

      Functional(const Functional& other, bool disableAssembly = false)
        : FunctionalBase< Functional<F,DF,DDF> >( other.domain_ptr() ) ,
          Mixin::DisableAssembly( disableAssembly ) ,
          f_( other.f_.mesh_shared_ptr() ) ,
          J_( other.J_.function_space(0) ) ,
          H_( other.H_.function_space(0) , other.H_.function_space(1) ) ,
          bcs_( other.bcs_ ),
          A_( (other.A_!=nullptr) ? other.A_->copy() : nullptr ) ,
          b_( (other.b_!=nullptr) ? other.b_->copy() : nullptr ) ,
          value_( other.value_ ) ,
          valueAssembled_( other.valueAssembled_ ) ,
          oldX_f_(other.oldX_f_) , oldX_J_(other.oldX_J_) , oldX_H_(other.oldX_H_) ,
          dummy_(J_.function_space(0)),
          solver_(other.solver_)
      {
        copyCoefficients(other.f_,f_);
        copyCoefficients(other.J_,J_);
        copyCoefficients(other.H_,H_);
      }


      double operator()(const ::Algorithm::Vector& x) const
      {
        primalDualIgnoreReset(std::bind(&Functional::assembleFunctional,std::ref(*this), std::placeholders::_1),x);

        return value_;
      }

      void setOrigin(const ::Algorithm::Vector& x) const
      {
        dummy_.vector()= std::make_shared<dolfin::Vector>(dummy_.vector()->mpi_comm(), dummy_.vector()->size());
        copy(x,*dummy_.vector());
        assign_x0_if_present(f_,dummy_);
        assign_x0_if_present(J_,dummy_);
        assign_x0_if_present(H_,dummy_);
      }

      LinearOperator hessian(const ::Algorithm::Vector& x) const
      {
        primalDualIgnoreReset(std::bind(&Functional::assembleHessian,std::ref(*this), std::placeholders::_1),x);

        assert( A_ != nullptr );
        return Hessian( Functional(*this,true), oldX_H_, solver_);
      }

      ::Algorithm::Vector d1_(const ::Algorithm::Vector &x) const
      {
        primalDualIgnoreReset(std::bind(&Functional::assembleJacobian,std::ref(*this), std::placeholders::_1),x);

        auto y = this->domain().dualSpace_ptr()->element();
        copy(*b_,y);
        return y;
      }

      ::Algorithm::Vector d2_(const ::Algorithm::Vector &x, const ::Algorithm::Vector &dx) const
      {
        primalDualIgnoreReset(std::bind(&Functional::assembleHessian,std::ref(*this), std::placeholders::_1),x);

        auto x_ = std::make_shared<dolfin::Vector>(dummy_.vector()->mpi_comm(), dummy_.vector()->size());
        copy(dx,*x_);
        auto Ax = x_->copy();
        A_->mult(*x_, *Ax);

        auto result = this->domain().dualSpace_ptr()->element();
        copy(*Ax,result);

        return result;
      }

    private:
      void assembleFunctional(const ::Algorithm::Vector& x) const
      {
        if( valueAssembled_ && (oldX_f_ == x) ) return;
        valueAssembled_ = true;
        dummy_.vector() = std::make_shared<dolfin::Vector>(dummy_.vector()->mpi_comm(), dummy_.vector()->size());
        copy(x,*dummy_.vector());

        f_.x = dummy_;
        value_ = dolfin::assemble(f_);

        oldX_f_ = x;
      }

      void assembleJacobian(const ::Algorithm::Vector& x) const
      {
        if( assemblyIsDisabled() ) return;

        if( b_ != nullptr && (oldX_J_==x) ) return;

        dummy_.vector() = std::make_shared<dolfin::Vector>(dummy_.vector()->mpi_comm(), dummy_.vector()->size());
        copy(x,*dummy_.vector());
        Assign_X_If_Present<decltype(J_)>::apply(J_,dummy_);
        b_ = dummy_.vector()->factory().create_vector();

        dolfin::assemble(*b_,J_);
        for( auto& bc : bcs_) bc->apply(*b_,*dummy_.vector());

        oldX_J_ = x;
      }

      void assembleHessian(const ::Algorithm::Vector& x) const
      {
        if( assemblyIsDisabled() ) return;

        if( A_ != nullptr && (oldX_H_==x) ) return;

        dummy_.vector() = std::make_shared<dolfin::Vector>(dummy_.vector()->mpi_comm(), dummy_.vector()->size());
        copy(x,*dummy_.vector());
        Assign_X_If_Present<decltype(H_)>::apply(H_,dummy_);
        A_ = dummy_.vector()->factory().create_matrix();
        dolfin::assemble(*A_,H_);

        for( auto& bc : bcs_) bc->apply(*A_,*dummy_.vector(),*dummy_.vector());

        oldX_H_ = x;
        solver_ = std::make_shared<LinearSolver>( LUSolver(A_,*J_.function_space(0),this->domain_ptr(),this->domain_ptr(),true) );
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
      mutable dolfin::Function dummy_;
      mutable std::shared_ptr<LinearSolver> solver_ = nullptr;
    };


    template <class F, class DF, class DDF>
    auto makeFunctional( const F& f , const DF& J , const DDF& H ,
                                            const std::vector<const dolfin::DirichletBC*>& bcs, ::Algorithm::VectorSpace& space)
    {
      return Fenics::Functional<F,DF,DDF>( f , J , H , bcs , space );
    }
  }
}

#endif // ALGORITHMS_ADAPTER_FENICS_FUNCTIONAL_HH

