#ifndef ALGORITHM_ADAPTER_FENICS_QUADRATIC_FUNCTIONAL_HH
#define ALGORITHM_ADAPTER_FENICS_QUADRATIC_FUNCTIONAL_HH

#include <memory>
#include <vector>

#include <dolfin.h>

#include "Interface/Functional/abstractC2Functional.hh"
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
    template <class BilinearForm>
    class QuadraticFunctional : public Interface::AbstractC2Functional
    {
    public:
      QuadraticFunctional(const BilinearForm& J, const std::vector<const dolfin::DirichletBC*>& bcs, std::shared_ptr<Interface::AbstractBanachSpace> space)
        : Interface::AbstractC2Functional( space ),
          J_( J.function_space(0) , J.function_space(1) ),
          bcs_( bcs )
      {
        copyCoefficients(J,J_);
      }

      QuadraticFunctional(const BilinearForm& J, const std::vector<const dolfin::DirichletBC*>& bcs, const BanachSpace& space)
        : QuadraticFunctional(J,bcs,space.sharedImpl())
      {}

      double d0(const Interface::AbstractFunctionSpaceElement& x) const final override
      {
        const auto& x_ = toVector(x);
        assembleJ(x_);

        auto y = clone(x_);
        A_->mult(*x_.impl().vector(), *y->impl().vector());

        return 0.5 * x_.impl().vector()->inner( *y->impl().vector() );
      }

      double d1(const Interface::AbstractFunctionSpaceElement &x, const Interface::AbstractFunctionSpaceElement &dx) const final override
      {
        const auto& x_ = toVector(x);
        const auto& dx_ = toVector(dx);
        assembleJ(x_);

        auto y = clone(x_);
        A_->mult(*x_.impl().vector(), *y->impl().vector());

        return dx_.impl().vector()->inner( *y->impl().vector() );
      }

      std::unique_ptr<Interface::AbstractFunctionSpaceElement> d2(const Interface::AbstractFunctionSpaceElement &x, const Interface::AbstractFunctionSpaceElement &dx) const final override
      {
        const auto& dx_ = toVector(dx);
        assembleJ(toVector(x));

        auto y = clone(dx_);
        A_->mult(*dx_.impl().vector(), *y->impl().vector());

        return std::unique_ptr<Interface::AbstractFunctionSpaceElement>( y.release() );
      }

    private:
      void assembleJ(const Vector& x) const
      {
        if( A_ != nullptr ) return;
        if( oldX_J != nullptr && oldX_J->equals(x) ) return;
        oldX_J = clone(x);

        A_ = x.impl().vector()->factory().create_matrix();

        // Assemble right-hand side
        dolfin::Assembler assembler;
        assembler.assemble(*A_, J_);

        // Apply boundary conditions
        for(const auto& bc : bcs_)
          bc->apply( *A_ );

        solver_ = std::make_shared<LUSolver>( *A_ );
      }

      QuadraticFunctional* cloneImpl() const
      {
        return new QuadraticFunctional(J_,bcs_,getSharedDomain());
      }

      Interface::Hessian makeHessian(const Interface::AbstractFunctionSpaceElement& x) const
      {
        assembleJ(toVector(x));
        return Interface::Hessian(clone(*this),x,solver_);
      }

      mutable BilinearForm J_;
      std::vector<const dolfin::DirichletBC*> bcs_;
      mutable std::shared_ptr<dolfin::GenericMatrix> A_ = nullptr;
      mutable std::shared_ptr<dolfin::GenericVector> b_ = nullptr;
      mutable std::unique_ptr<Vector> oldX_F, oldX_J = nullptr;
    };


    template <class BilinearForm>
    auto makeQuadraticFunctional(const BilinearForm& J, const std::vector<const dolfin::DirichletBC*>& bcs, const BanachSpace& space)
    {
      return ::Algorithm::makeC2Functional< ::Algorithm::Fenics::QuadraticFunctional<BilinearForm> >( J , bcs , space );
    }
  }
}

#endif // ALGORITHM_ADAPTER_FENICS_QUADRATIC_FUNCTIONAL_HH
