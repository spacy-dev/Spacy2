#ifndef ALGORITHM_ADAPTER_FENICS_QUADRATIC_FUNCTIONAL_HH
#define ALGORITHM_ADAPTER_FENICS_QUADRATIC_FUNCTIONAL_HH

#include <memory>
#include <vector>

#include <dolfin.h>

#include "Interface/Functional/hessian.hh"

#include "luSolver.hh"
#include "util.hh"
#include "vector.hh"

#include "banachSpace.hh"
#include "../../c2Functional.hh"

namespace Algorithm
{
  namespace Fenics
  {
    template <class Functional, class FirstDerivative, class SecondDerivative>
    class QuadraticFunctional : public Interface::AbstractC2Functional
    {
    public:
      QuadraticFunctional(const Functional& f, const FirstDerivative& J, const SecondDerivative& H,
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

      double d0(const Interface::AbstractFunctionSpaceElement& x) const final override
      {
        if( oldX_f_ != nullptr && oldX_f_->equals(x) ) return value;

        oldX_f_ = clone(x);

        readDummy(x);
        f_.x = dummy_;
        value = dolfin::assemble(f_);

        return value;
      }

      double d1(const Interface::AbstractFunctionSpaceElement &x, const Interface::AbstractFunctionSpaceElement &dx) const final override
      {
        const auto& x_ = toProductSpaceElement(x);
        assemble_J(x_);

        readDummy(dx);

        return b_->inner( *dummy_.vector() );
      }

      std::unique_ptr<Interface::AbstractFunctionSpaceElement> d2(const Interface::AbstractFunctionSpaceElement &x, const Interface::AbstractFunctionSpaceElement &dx) const final override
      {
        const auto& x_ = toProductSpaceElement(x);
        assemble_H(x_);

        readDummy(dx);
        auto Ax = dummy_;
        A_->mult(*dummy_.vector(), *Ax.vector());

        auto result = clone(x_);

        for(auto i=0u; i<result->variables().size(); ++i)
          toVector(result->variable(i)).impl() = Ax[i];

        return std::unique_ptr<Interface::AbstractFunctionSpaceElement>( result.release() );
      }

      std::shared_ptr<Interface::AbstractLinearSolver> createLinearSolver(const std::string& name, const Interface::AbstractFunctionSpaceElement& x) const override
      {
        assemble_H(toProductSpaceElement(x));
        return makeLinearSolver(name,*A_);
      }

    private:
      void readDummy(const Interface::AbstractFunctionSpaceElement& x) const
      {
        const auto& x_ = toProductSpaceElement(x);
        for( auto i=0u; i<x_.variables().size(); ++i)
          dummy_[i] = toVector( x_.variable(i) ).impl();

        if( !x_.isPrimalEnabled() )
          for( auto i : x_.space().primalSubSpaceIds() )
            *dummy_[i].vector() *= 0.;
        if( !x_.isDualEnabled() )
          for( auto i : x_.space().dualSubSpaceIds() )
            *dummy_[i].vector() *= 0.;
      }

      void assemble_J(const ProductSpaceElement& x) const
      {
        if( oldX_J_ != nullptr && oldX_J_->equals(x) ) return;
        oldX_J_ = clone(x);

        readDummy(x);
        J_.x = dummy_;
        b_ = dummy_.vector()->factory().create_vector();

        dolfin::assemble(*b_,J_);
        for( auto& bc : bcs_) bc->apply(*b_);
      }

      void assemble_H(const ProductSpaceElement& x) const
      {
        if( oldX_H_ != nullptr && oldX_H_->equals(x) ) return;
        oldX_H_ = clone(x);

        readDummy(x);
        A_ = dummy_.vector()->factory().create_matrix();
        dolfin::assemble(*A_,H_);

        for( auto& bc : bcs_) bc->apply(*A_);
      }

      QuadraticFunctional* cloneImpl() const
      {
        return new QuadraticFunctional(f_,J_,H_,bcs_,getSharedDomain());
      }

      Interface::Hessian makeHessian(const Interface::AbstractFunctionSpaceElement& x) const
      {
        assembleJ(toVector(x));
        return Interface::Hessian(clone(*this),x,solver_);
      }

      mutable Functional f_;
      mutable FirstDerivative J_;
      mutable SecondDerivative H_;
      const std::vector<const dolfin::DirichletBC*>& bcs_;
      mutable std::shared_ptr<dolfin::GenericMatrix> A_;
      mutable std::shared_ptr<dolfin::GenericVector> b_;
      mutable std::unique_ptr<Interface::AbstractFunctionSpaceElement> oldX_f_, oldX_J_, oldX_H_;
      mutable double value = 0;
      mutable dolfin::Function dummy_;
    };


    template <class BilinearForm>
    auto makeQuadraticFunctional(const BilinearForm& J, const std::vector<const dolfin::DirichletBC*>& bcs, const BanachSpace& space)
    {
      return ::Algorithm::makeC2Functional< ::Algorithm::Fenics::QuadraticFunctional<BilinearForm> >( J , bcs , space );
    }
  }
}

#endif // ALGORITHM_ADAPTER_FENICS_QUADRATIC_FUNCTIONAL_HH
