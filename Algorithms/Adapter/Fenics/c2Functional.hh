#ifndef ALGORITHMS_ADAPTER_FENICS_C2FUNCTIONAL_HH
#define ALGORITHMS_ADAPTER_FENICS_C2FUNCTIONAL_HH

#include <chrono>
#include <memory>
#include <vector>

#include "FunctionSpaces/ProductSpace/productSpaceElement.hh"
#include "Interface/Functional/abstractC2Functional.hh"
#include "Interface/Functional/hessian.hh"
#include "Util/Mixins/disableAssembly.hh"

#include "../../c2Functional.hh"

#include "util.hh"
#include "vector.hh"
#include "assignXIfPresent.hh"

namespace Algorithm
{
  namespace Fenics
  {
    template <class Functional, class FirstDerivative, class SecondDerivative>
    class C2Functional : public Interface::AbstractC2Functional , public Mixin::DisableAssembly
    {
    public:
      C2Functional(const Functional& f, const FirstDerivative& J, const SecondDerivative& H,
                   const std::vector<const dolfin::DirichletBC*>& bcs, std::shared_ptr<Interface::AbstractBanachSpace> space)
        : Interface::AbstractC2Functional( space ),
          f_( J.function_space(0)->mesh() ),
          J_( J.function_space(0) ),
          H_( H.function_space(0) , H.function_space(1) ),
          bcs_( bcs ),
          dummy_(J_.function_space(0))
      {
        copyCoefficients(f,f_);
        copyCoefficients(J,J_);
        copyCoefficients(H,H_);
      }

      C2Functional(const Functional& f, const FirstDerivative& J, const SecondDerivative& H,
                   const std::vector<const dolfin::DirichletBC*>& bcs, const BanachSpace& space)
        : C2Functional(f,J,H,bcs,space.sharedImpl())
      {}

      C2Functional(const Functional& f, const FirstDerivative& J, const SecondDerivative& H,
                   const std::vector<const dolfin::DirichletBC*>& bcs, std::shared_ptr<Interface::AbstractBanachSpace> space,
                   const dolfin::GenericMatrix& A,
                   const Interface::AbstractFunctionSpaceElement& oldX_H)
        : Interface::AbstractC2Functional( space ),
          Mixin::DisableAssembly(true),
          f_( J.function_space(0)->mesh() ),
          J_( J.function_space(0) ),
          H_( H.function_space(0) , H.function_space(1) ),
          bcs_( bcs ),
          A_(A.copy()),
          oldX_H_(clone(oldX_H)),
          dummy_(J_.function_space(0))
      {
        copyCoefficients(f,f_);
        copyCoefficients(J,J_);
        copyCoefficients(H,H_);
      }


      double d0(const Interface::AbstractFunctionSpaceElement& x) const final override
      {
        bool dualEnabled = (isProductSpaceElement(x)) ? toProductSpaceElement(x).isDualEnabled() : true;
        bool primalEnabled = (isProductSpaceElement(x)) ? toProductSpaceElement(x).isPrimalEnabled() : true;
        if( oldX_f_ != nullptr && oldX_f_->equals(x) ) return value_;
        if( !dualEnabled ) primal(x);
        if( !primalEnabled ) dual(x);

        auto y_ = std::make_shared<dolfin::Vector>(dummy_.vector()->mpi_comm(), dummy_.vector()->size());
        copy(x,*y_);

        *dummy_.vector() = *y_;

        f_.x = dummy_;
        value_ = dolfin::assemble(f_);

        oldX_f_ = clone(x);
        return value_;
      }

      std::unique_ptr<Interface::AbstractFunctionSpaceElement> d1(const Interface::AbstractFunctionSpaceElement &x) const final override
      {
        assembleJacobian(x);

        auto y = clone(x);
        copy(*b_,*y);
        return std::move(y);
      }

      std::unique_ptr<Interface::AbstractFunctionSpaceElement> d2(const Interface::AbstractFunctionSpaceElement &x, const Interface::AbstractFunctionSpaceElement &dx) const final override
      {
        assembleHessian(x);

        auto y_ = std::make_shared<dolfin::Vector>(dummy_.vector()->mpi_comm(), dummy_.vector()->size());
        copy(dx,*y_);
        auto Ax = y_->copy();
        A_->mult(*y_, *Ax);

        auto result = clone(x);
        copy(*Ax,*result);

        return std::unique_ptr<Interface::AbstractFunctionSpaceElement>( result.release() );
      }

      void setOrigin(const Interface::AbstractFunctionSpaceElement& x) const
      {
        auto y_ = std::make_shared<dolfin::Vector>(dummy_.vector()->mpi_comm(), dummy_.vector()->size());
        copy(x,*y_);
        *dummy_.vector() = *y_;
        assign_x0_if_present(f_,dummy_);
        assign_x0_if_present(J_,dummy_);
        assign_x0_if_present(H_,dummy_);
      }

    private:
      std::unique_ptr<Interface::Hessian> makeHessian(const Interface::AbstractFunctionSpaceElement& x) const override
      {
        assembleHessian(x);

        assert( A_ != nullptr );
        return std::make_unique<Interface::Hessian>( std::make_unique<C2Functional>(f_,J_,H_,bcs_,sharedDomain(),*A_,*oldX_H_), *oldX_H_);
      }

      std::unique_ptr<Interface::AbstractLinearSolver> makeSolver() const
      {
        assert( A_ != nullptr );
        return std::make_unique<LUSolver>(A_,*J_.function_space(0),sharedDomain(),sharedDomain());
      }

      void assembleJacobian(const Interface::AbstractFunctionSpaceElement& x) const
      {
        if( assemblyIsDisabled() ) return;

        bool dualEnabled = (isProductSpaceElement(x)) ? toProductSpaceElement(x).isDualEnabled() : true;
        bool primalEnabled = (isProductSpaceElement(x)) ? toProductSpaceElement(x).isPrimalEnabled() : true;
        if( oldX_J_ != nullptr && oldX_J_->equals(x) ) return;
        if( !dualEnabled ) primal(x);
        if( !primalEnabled ) dual(x);

        auto y_ = std::make_shared<dolfin::Vector>(dummy_.vector()->mpi_comm(), dummy_.vector()->size());
        copy(x,*y_);
        *dummy_.vector() = *y_;
        Assign_X_If_Present<decltype(J_)>::apply(J_,dummy_);
        b_ = dummy_.vector()->factory().create_vector();

        dolfin::assemble(*b_,J_);
        for( auto& bc : bcs_) bc->apply(*b_,*dummy_.vector());

        oldX_J_ = clone(x);
      }

      void assembleHessian(const Interface::AbstractFunctionSpaceElement& x) const
      {
        if( assemblyIsDisabled() ) return;

        bool dualEnabled = (isProductSpaceElement(x)) ? toProductSpaceElement(x).isDualEnabled() : true;
        bool primalEnabled = (isProductSpaceElement(x)) ? toProductSpaceElement(x).isPrimalEnabled() : true;
        if( oldX_H_ != nullptr && oldX_H_->equals(x) ) return;
        if( !dualEnabled ) primal(x);
        if( !primalEnabled ) dual(x);

        auto y_ = std::make_shared<dolfin::Vector>(dummy_.vector()->mpi_comm(), dummy_.vector()->size());
        copy(x,*y_);
        *dummy_.vector() = *y_;
        Assign_X_If_Present<decltype(H_)>::apply(H_,dummy_);
        A_ = dummy_.vector()->factory().create_matrix();
        dolfin::assemble(*A_,H_);

        for( auto& bc : bcs_) bc->apply(*A_);

        oldX_H_ = clone(x);
      }

      C2Functional* cloneImpl() const
      {
        if( assemblyIsDisabled() ) return new C2Functional(f_,J_,H_,bcs_,sharedDomain(),*A_,*oldX_H_);
        return new C2Functional(f_,J_,H_,bcs_,sharedDomain());
      }

      mutable Functional f_;
      mutable FirstDerivative J_;
      mutable SecondDerivative H_;
      const std::vector<const dolfin::DirichletBC*>& bcs_;
      mutable std::shared_ptr<dolfin::GenericMatrix> A_;
      mutable std::shared_ptr<dolfin::GenericVector> b_;
      mutable double value_ = 0;
      mutable std::unique_ptr<Interface::AbstractFunctionSpaceElement> oldX_f_, oldX_J_, oldX_H_;
      mutable dolfin::Function dummy_;
    };


    template <class Functional, class Derivative, class Hessian, class... Args>
    ::Algorithm::C2Functional makeC2Functional( const Functional& f , const Derivative& J , const Hessian& H , Args&&... args )
    {
      return createFromUniqueImpl< ::Algorithm::C2Functional , Fenics::C2Functional<Functional,Derivative,Hessian> >( f , J , H , std::forward<Args>(args)...);
    }
  }
}

#endif // ALGORITHMS_ADAPTER_FENICS_C2FUNCTIONAL_HH

