#ifndef ALGORITHM_ADAPTER_FENICS_NORM_OPERATOR_HH
#define ALGORITHM_ADAPTER_FENICS_NORM_OPERATOR_HH

#include <memory>
#include <vector>

#include <dolfin.h>

#include "Interface/Operator/abstractOperator.hh"
#include "Interface/Operator/linearizedOperator.hh"

#include "luSolver.hh"
#include "vector.hh"

#include "banachSpace.hh"
#include "../../operator.hh"
#include "Util/create.hh"

#include "assignXIfPresent.hh"

namespace Algorithm
{
  namespace Fenics
  {
    template <class JacobianForm>
    class NormOperator : public Interface::AbstractOperator
    {
    public:
      NormOperator(const JacobianForm& J, const std::vector<const dolfin::DirichletBC*>& bcs,
                 std::shared_ptr<Interface::AbstractBanachSpace> domain,
                 std::shared_ptr<Interface::AbstractBanachSpace> range)
        : Interface::AbstractOperator( domain , range ),
          J_( J.function_space(0) , J.function_space(1) ),
          bcs_( bcs )
      {
        copyCoefficients(J,J_);
        assemble(*domain->element());
      }

      NormOperator(const JacobianForm& J, const std::vector<const dolfin::DirichletBC*>& bcs,
                 std::shared_ptr<dolfin::GenericMatrix> A,
                 std::shared_ptr<Interface::AbstractBanachSpace> domain,
                 std::shared_ptr<Interface::AbstractBanachSpace> range)
        : Interface::AbstractOperator( domain , range ),
          J_( J.function_space(0) , J.function_space(1) ),
          bcs_( bcs ),
          A_(A->copy())
      {
        copyCoefficients(J,J_);
      }

      NormOperator(const JacobianForm& J, const std::vector<const dolfin::DirichletBC*>& bcs,
                 const BanachSpace& domain, const BanachSpace& range)
        : NormOperator(J,bcs,domain.sharedImpl(),range.sharedImpl())
      {}

      std::unique_ptr<Interface::AbstractFunctionSpaceElement> operator()(const Interface::AbstractFunctionSpaceElement& x) const final override
      {
        auto result = range().element();
        A_->mult( *toVector(x).impl().vector() , *toVector(*result).impl().vector() );

        return std::move(result);
      }

    private:
      friend class LinearizedOperator;

      void assemble(const Interface::AbstractFunctionSpaceElement& x) const
      {
        dolfin::Function dummy_( J_.function_space(0) );
        auto y_ = std::make_shared<dolfin::Vector>(dummy_.vector()->mpi_comm(), dummy_.vector()->size());
        copy(x,*y_);
        *dummy_.vector() = *y_;
        Assign_X_If_Present<decltype(J_)>::apply(J_,dummy_);
        A_ = dummy_.vector()->factory().create_matrix();

        // Assemble right-hand side
        dolfin::Assembler assembler;
        assembler.assemble(*A_, J_);

        // Apply boundary conditions
        for(const auto& bc : bcs_)
          bc->apply( *A_ );
      }

      NormOperator* cloneImpl() const
      {
        return new NormOperator(J_,bcs_,A_,sharedDomain(),sharedRange());
      }

      std::unique_ptr<Interface::LinearizedOperator> makeLinearization(const Interface::AbstractFunctionSpaceElement& x) const
      {
        assemble(x);
        return std::make_unique<Interface::LinearizedOperator>(clone(this),x);
      }

      std::unique_ptr<Interface::AbstractLinearSolver> makeSolver() const
      {
        assert (A_ != nullptr);
        return std::make_unique<LUSolver>( *A_ , *J_.function_space(0) , sharedRange() , sharedDomain() );
      }

      mutable JacobianForm J_;
      std::vector<const dolfin::DirichletBC*> bcs_;
      mutable std::shared_ptr<dolfin::GenericMatrix> A_ = nullptr;
    };


    template <class JacobianForm>
    auto makeNormOperator(JacobianForm& J, const std::vector<const dolfin::DirichletBC*>& bcs, const BanachSpace& domain, const BanachSpace& range)
    {
      return create_uniqueImpl< ::Algorithm::Operator , ::Algorithm::Fenics::NormOperator<JacobianForm> >( J , bcs , domain , range );
    }
  }
}

#endif // ALGORITHM_ADAPTER_FENICS_NORM_OPERATOR_HH
