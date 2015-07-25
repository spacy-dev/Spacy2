#ifndef ALGORITHM_ADAPTER_FENICS_OPERATOR_HH
#define ALGORITHM_ADAPTER_FENICS_OPERATOR_HH

#include <memory>
#include <vector>

#include <dolfin.h>

#include "Interface/Operator/abstractOperator.hh"

#include "luSolver.hh"
#include "util.hh"
#include "vector.hh"

#include "banachSpace.hh"
#include "../../operator.hh"
#include "Util/create.hh"

namespace Algorithm
{
  namespace Fenics
  {
    template <class JacobianForm>
    class Operator : public Interface::AbstractOperator
    {
    public:
      Operator(const JacobianForm& J, const std::vector<const dolfin::DirichletBC*>& bcs, std::shared_ptr<Interface::AbstractBanachSpace> space)
        : Interface::AbstractOperator( space , space ),
          J_( J.function_space(0) , J.function_space(1) ),
          bcs_( bcs )
      {
        copyCoefficients(J,J_);
        assemble();
      }

      Operator(const JacobianForm& J, const std::vector<const dolfin::DirichletBC*>& bcs, const BanachSpace& space)
        : Operator(J,bcs,space.sharedImpl())
      {}

      std::unique_ptr<Interface::AbstractFunctionSpaceElement> operator()(const Interface::AbstractFunctionSpaceElement& x) const final override
      {
        const auto& x_ = toVector(x);

        auto y = clone(x_);
        A_->mult(*x_.impl().vector(), *y->impl().vector());

        return std::unique_ptr<Interface::AbstractFunctionSpaceElement>( y.release() );
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

      Operator* cloneImpl() const
      {
        return new Operator(J_,bcs_,sharedDomain());
      }

      mutable JacobianForm J_;
      std::vector<const dolfin::DirichletBC*> bcs_;
      mutable std::shared_ptr<dolfin::GenericMatrix> A_ = nullptr;
    };


    template <class JacobianForm>
    auto makeOperator(const JacobianForm& J, const std::vector<const dolfin::DirichletBC*>& bcs, std::shared_ptr<Interface::AbstractBanachSpace> space)
    {
      return create_uniqueImpl< ::Algorithm::Operator , ::Algorithm::Fenics::Operator<JacobianForm> >( J , bcs , space );
    }
  }
}

#endif // ALGORITHM_ADAPTER_FENICS_OPERATOR_HH
