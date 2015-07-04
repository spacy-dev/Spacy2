#ifndef ALGORITHM_ADAPTER_FENICS_C1OPERATOR_HH
#define ALGORITHM_ADAPTER_FENICS_C1OPERATOR_HH

#include <memory>
#include <vector>

#include <dolfin.h>

#include "Interface/Operator/abstractC1Operator.hh"
#include "Interface/Operator/linearizedOperator.hh"

#include "luSolver.hh"
#include "util.hh"
#include "vector.hh"

#include "banachSpace.hh"
#include "../../c1Operator.hh"

namespace Algorithm
{
  namespace Fenics
  {
    template <class ResidualForm, class JacobianForm>
    class C1Operator : public Interface::AbstractC1Operator
    {
    public:
      C1Operator(const ResidualForm& F, const JacobianForm& J, const std::vector<const dolfin::DirichletBC*>& bcs, std::shared_ptr<Interface::AbstractBanachSpace> space)
        : Interface::AbstractC1Operator( space , space ),
          F_( F.function_space(0) ),
          J_( J.function_space(0) , J.function_space(1) ),
          bcs_( bcs )
      {
        copyCoefficients(F,F_);
        copyCoefficients(J,J_);
      }

      C1Operator(const ResidualForm& F, const JacobianForm& J, const std::vector<const dolfin::DirichletBC*>& bcs, const BanachSpace& space)
        : C1Operator(F,J,bcs,space.sharedImpl())
      {}

      std::unique_ptr<Interface::AbstractFunctionSpaceElement> operator()(const Interface::AbstractFunctionSpaceElement& x) const final override
      {
        const auto& x_ = toVector(x);
        assembleF(x_);
        auto y = clone(x_);
        *y->impl().vector() = *b_;

        return std::unique_ptr<Interface::AbstractFunctionSpaceElement>( y.release() );
      }

      std::unique_ptr<Interface::AbstractFunctionSpaceElement> d1(const Interface::AbstractFunctionSpaceElement &x, const Interface::AbstractFunctionSpaceElement &dx) const final override
      {
        const auto& x_ = toVector(x);
        const auto& dx_ = toVector(dx);

        assembleJ(x_);
        auto y = clone(x_);
        A_->mult(*dx_.impl().vector(), *y->impl().vector());

        return std::unique_ptr<Interface::AbstractFunctionSpaceElement>( y.release() );
      }

    private:
      void assembleF(const Vector& x) const
      {
        if( oldX_F != nullptr && oldX_F->equals(x) ) return;
        oldX_F = clone(x);

        F_.u = x.impl();
        b_ = x.impl().vector()->factory().create_vector();

        // Assemble right-hand side
        dolfin::Assembler assembler;
        assembler.assemble(*b_, F_);

        // Apply boundary conditions
        for(const auto& bc : bcs_)
          bc->apply( *b_ , *x.impl().vector() );
      }

      void assembleJ(const Vector& x) const
      {
        if( oldX_J != nullptr && oldX_J->equals(x) ) return;
        oldX_J = clone(x);

        J_.u = x.impl();
        A_ = x.impl().vector()->factory().create_matrix();

        // Assemble right-hand side
        dolfin::Assembler assembler;
        assembler.assemble(*A_, J_);

        // Apply boundary conditions
        for(const auto& bc : bcs_)
          bc->apply( *A_ );

        solver_ = std::make_shared<LUSolver>( *A_ );
      }

      C1Operator* cloneImpl() const
      {
        return new C1Operator(F_,J_,bcs_,getSharedDomain());
      }

      Interface::LinearizedOperator makeLinearization(const Interface::AbstractFunctionSpaceElement& x) const
      {
        assembleF(toVector(x));
        assembleJ(toVector(x));
        return Interface::LinearizedOperator(clone(*this),x,solver_);
      }

      mutable ResidualForm F_;
      mutable JacobianForm J_;
      std::vector<const dolfin::DirichletBC*> bcs_;
      mutable std::shared_ptr<dolfin::GenericMatrix> A_ = nullptr;
      mutable std::shared_ptr<dolfin::GenericVector> b_ = nullptr;
      mutable std::unique_ptr<Vector> oldX_F, oldX_J = nullptr;
    };


    template <class ResidualForm, class JacobianForm>
    auto makeC1Operator(ResidualForm& F, JacobianForm& J, const std::vector<const dolfin::DirichletBC*>& bcs, const BanachSpace& space)
    {
      return ::Algorithm::makeC1Operator< ::Algorithm::Fenics::C1Operator<ResidualForm,JacobianForm> >( F , J , bcs , space );
    }
  }
}

#endif // ALGORITHM_ADAPTER_FENICS_C1OPERATOR_HH
