#ifndef ALGORITHM_ADAPTER_FENICS_LINEAR_OPERATOR_HH
#define ALGORITHM_ADAPTER_FENICS_LINEAR_OPERATOR_HH

#include <memory>
#include <vector>

#include <dolfin.h>

#include "Interface/Operator/abstractC1Operator.hh"

#include "luSolver.hh"
#include "util.hh"
#include "vector.hh"

#include "banachSpace.hh"
#include "../../c1Operator.hh"
#include "Util/create.hh"

namespace Algorithm
{
  namespace Fenics
  {
    template <class JacobianForm>
    class LinearOperator : public Interface::AbstractC1Operator
    {
    public:
      LinearOperator(const JacobianForm& J, const std::vector<const dolfin::DirichletBC*>& bcs, std::shared_ptr<Interface::AbstractBanachSpace> space)
        : Interface::AbstractC1Operator( space , space ),
          J_( J.function_space(0) , J.function_space(1) ),
          bcs_( bcs )
      {
        copyCoefficients(J,J_);
        assemble();
      }

      LinearOperator(const JacobianForm& J, const std::vector<const dolfin::DirichletBC*>& bcs, const BanachSpace& space)
        : LinearOperator(J,bcs,space.sharedImpl())
      {}

      std::unique_ptr<Interface::AbstractFunctionSpaceElement> operator()(const Interface::AbstractFunctionSpaceElement& x) const final override
      {
        const auto& x_ = toVector(x);

        auto y = clone(x_);
        A_->mult(*x_.impl().vector(), *y->impl().vector());

        return std::unique_ptr<Interface::AbstractFunctionSpaceElement>( y.release() );
      }

      std::unique_ptr<Interface::AbstractFunctionSpaceElement> d1(const Interface::AbstractFunctionSpaceElement&,
                                                                  const Interface::AbstractFunctionSpaceElement& dx) const final override
      {
        return (*this)(dx);
      }


    private:
      void assemble() const
      {
        auto x = domain().element();

        A_ = toVector(*x).impl().vector()->factory().create_matrix();

        // Assemble right-hand side
        dolfin::Assembler assembler;
        assembler.assemble(*A_, J_);

        // Apply boundary conditions
        for(const auto& bc : bcs_)
          bc->apply( *A_ );
      }

      LinearOperator* cloneImpl() const
      {
        return new LinearOperator(J_,bcs_,sharedDomain());
      }

      mutable JacobianForm J_;
      std::vector<const dolfin::DirichletBC*> bcs_;
      mutable std::shared_ptr<dolfin::GenericMatrix> A_ = nullptr;
    };


    template <class JacobianForm>
    auto makeLinearC1Operator(const JacobianForm& J, const std::vector<const dolfin::DirichletBC*>& bcs, std::shared_ptr<Interface::AbstractBanachSpace> space)
    {
      return create_uniqueImpl< ::Algorithm::C1Operator , ::Algorithm::Fenics::LinearOperator<JacobianForm> >( J , bcs , space );
    }
  }
}

#endif // ALGORITHM_ADAPTER_FENICS_LINEAR_OPERATOR_HH
