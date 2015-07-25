#ifndef ALGORITHM_ADAPTER_FENICS_FAKE_C2OPERATOR_HH
#define ALGORITHM_ADAPTER_FENICS_FAKE_C2OPERATOR_HH

#include <memory>
#include <vector>

#include <dolfin.h>

#include "Interface/Operator/abstractC2Operator.hh"
#include "Interface/Operator/linearizedOperator.hh"

#include "luSolver.hh"
#include "util.hh"
#include "vector.hh"

#include "banachSpace.hh"
#include "../../c2Operator.hh"
#include "Util/Exceptions/callOfUndefinedFunctionException.hh"
#include "Util/create.hh"

namespace Algorithm
{
  namespace Fenics
  {
    template <class ResidualForm, class JacobianForm>
    class FakeC2Operator : public Interface::AbstractC2Operator
    {
    public:
      FakeC2Operator(const ResidualForm& F, const JacobianForm& J, const std::vector<const dolfin::DirichletBC*>& bcs, std::shared_ptr<Interface::AbstractBanachSpace> space)
        : Interface::AbstractC2Operator( space , space ),
          F_( F.function_space(0) ),
          J_( J.function_space(0) , J.function_space(1) ),
          bcs_( bcs )
      {
        copyCoefficients(F,F_);
        copyCoefficients(J,J_);
      }

      FakeC2Operator(const ResidualForm& F, const JacobianForm& J, const std::vector<const dolfin::DirichletBC*>& bcs, const BanachSpace& space)
        : FakeC2Operator(F,J,bcs,space.sharedImpl())
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

      std::unique_ptr<Interface::AbstractFunctionSpaceElement> d2(const Interface::AbstractFunctionSpaceElement& x,
                                                                  const Interface::AbstractFunctionSpaceElement& dx,
                                                                  const Interface::AbstractFunctionSpaceElement& dy) const final override
      {
        throw CallOfUndefinedFunctionException("FakeC2Operator::d2");
      }

    private:
      friend class LinearizedOperator;

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
      }

      FakeC2Operator* cloneImpl() const
      {
        return new FakeC2Operator(F_,J_,bcs_,sharedDomain());
      }

      Interface::LinearizedOperator makeLinearization(const Interface::AbstractFunctionSpaceElement& x) const
      {
        assembleF(toVector(x));
        assembleJ(toVector(x));
        return Interface::LinearizedOperator(clone(*this),x);
      }

      std::unique_ptr<Interface::AbstractLinearSolver> makeSolver() const
      {
        return std::make_unique<LUSolver>( *A_ );
      }

      mutable ResidualForm F_;
      mutable JacobianForm J_;
      std::vector<const dolfin::DirichletBC*> bcs_;
      mutable std::shared_ptr<dolfin::GenericMatrix> A_ = nullptr;
      mutable std::shared_ptr<dolfin::GenericVector> b_ = nullptr;
      mutable std::unique_ptr<Vector> oldX_F, oldX_J = nullptr;
    };


    template <class ResidualForm, class JacobianForm>
    auto makeFakeC2Operator(ResidualForm& F, JacobianForm& J, const std::vector<const dolfin::DirichletBC*>& bcs, const BanachSpace& space)
    {
      return create_uniqueImpl< ::Algorithm::C2Operator , ::Algorithm::Fenics::FakeC2Operator<ResidualForm,JacobianForm> >( F , J , bcs , space );
    }
  }
}

#endif // ALGORITHM_ADAPTER_FENICS_C1OPERATOR_HH
