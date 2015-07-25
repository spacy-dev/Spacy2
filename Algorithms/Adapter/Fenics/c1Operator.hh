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

#include "hilbertSpace.hh"
#include "banachSpace.hh"
#include "../../c1Operator.hh"
#include "Util/create.hh"
#include "Util/makeLinearSolver.hh"
#include "Util/Mixins/disableAssembly.hh"

#include "assignXIfPresent.hh"

namespace Algorithm
{
  namespace Fenics
  {
    template <class ResidualForm, class JacobianForm>
    class C1Operator : public Interface::AbstractC1Operator , public Mixin::DisableAssembly
    {
    public:
      C1Operator(const ResidualForm& F, const JacobianForm& J, const std::vector<const dolfin::DirichletBC*>& bcs,
                 std::shared_ptr<Interface::AbstractBanachSpace> domain,
                 std::shared_ptr<Interface::AbstractBanachSpace> range)
        : Interface::AbstractC1Operator( domain , range ),
          F_( F.function_space(0) ),
          J_( J.function_space(0) , J.function_space(1) ),
          bcs_( bcs ),
          dummy_( J.function_space(0) )
      {
        copyCoefficients(F,F_);
        copyCoefficients(J,J_);
      }

      C1Operator(const ResidualForm& F, const JacobianForm& J, const std::vector<const dolfin::DirichletBC*>& bcs,
                 std::shared_ptr<dolfin::GenericMatrix> A,
                 std::shared_ptr<Interface::AbstractBanachSpace> domain,
                 std::shared_ptr<Interface::AbstractBanachSpace> range)
        : Interface::AbstractC1Operator( domain , range ),
          Mixin::DisableAssembly(true),
          F_( F.function_space(0) ),
          J_( J.function_space(0) , J.function_space(1) ),
          bcs_( bcs ),
          A_(A->copy()),
          dummy_( J.function_space(0) )
      {
        copyCoefficients(F,F_);
        copyCoefficients(J,J_);
      }

      C1Operator(const ResidualForm& F, const JacobianForm& J, const std::vector<const dolfin::DirichletBC*>& bcs,
                 const BanachSpace& domain, const BanachSpace& range)
        : C1Operator(F,J,bcs,domain.sharedImpl(),range.sharedImpl())
      {}

      std::unique_ptr<Interface::AbstractFunctionSpaceElement> operator()(const Interface::AbstractFunctionSpaceElement& x) const final override
      {
        assembleF(x);

        auto y = clone(x);
        copy(*b_,*y);
        return std::move(y);
      }

      std::unique_ptr<Interface::AbstractFunctionSpaceElement> d1(const Interface::AbstractFunctionSpaceElement &x, const Interface::AbstractFunctionSpaceElement &dx) const final override
      {
        assembleJ(x);

        auto y_ = std::make_shared<dolfin::Vector>(dummy_.vector()->mpi_comm(), dummy_.vector()->size());
        copy(dx,*y_);
        auto Ax = y_->copy();
        A_->mult(*y_, *Ax);

        auto result = clone(x);
        copy(*Ax,*result);

        return std::unique_ptr<Interface::AbstractFunctionSpaceElement>( result.release() );
      }

    private:
      friend class LinearizedOperator;

      void assembleF(const Interface::AbstractFunctionSpaceElement& x) const
      {
        if( assemblyIsDisabled() ) return;
        if( oldX_F != nullptr && oldX_F->equals(x) ) return;
        oldX_F = clone(x);

        auto y_ = std::make_shared<dolfin::Vector>(dummy_.vector()->mpi_comm(), dummy_.vector()->size());
        copy(x,*y_);
        *dummy_.vector() = *y_;
        Assign_X_If_Present<decltype(F_)>::apply(F_,dummy_);
        b_ = dummy_.vector()->factory().create_vector();

        // Assemble right-hand side
        dolfin::Assembler assembler;
        assembler.assemble(*b_, F_);

        // Apply boundary conditions
        for(const auto& bc : bcs_)
          bc->apply( *b_ , *dummy_.vector() );
      }

      void assembleJ(const Interface::AbstractFunctionSpaceElement& x) const
      {
        if( assemblyIsDisabled() ) return;
        if( oldX_J != nullptr && oldX_J->equals(x) ) return;
        oldX_J = clone(x);

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

      C1Operator* cloneImpl() const
      {
        if( assemblyIsDisabled() ) return new C1Operator(F_,J_,bcs_,A_,sharedDomain(),sharedRange());
        return new C1Operator(F_,J_,bcs_,sharedDomain(),sharedRange());
      }

      std::unique_ptr<Interface::LinearizedOperator> makeLinearization(const Interface::AbstractFunctionSpaceElement& x) const
      {
        assembleF(x);
        assembleJ(x);
        return std::make_unique<Interface::LinearizedOperator>(std::make_unique<C1Operator>(F_,J_,bcs_,A_,sharedDomain(),sharedRange()),x);
      }

      std::unique_ptr<Interface::AbstractLinearSolver> makeSolver() const
      {
        assert (A_ != nullptr);
        return std::make_unique<LUSolver>( A_ , *F_.function_space(0) , sharedRange() , sharedDomain() );
      }

      mutable ResidualForm F_;
      mutable JacobianForm J_;
      std::vector<const dolfin::DirichletBC*> bcs_;
      mutable std::shared_ptr<dolfin::GenericMatrix> A_ = nullptr;
      mutable std::shared_ptr<dolfin::GenericVector> b_ = nullptr;
      mutable std::unique_ptr<Interface::AbstractFunctionSpaceElement> oldX_F, oldX_J = nullptr;
      mutable dolfin::Function dummy_;
    };


    template <class ResidualForm, class JacobianForm>
    auto makeC1Operator(ResidualForm& F, JacobianForm& J, const std::vector<const dolfin::DirichletBC*>& bcs, const BanachSpace& domain, const BanachSpace& range)
    {
      return create_uniqueImpl< ::Algorithm::C1Operator , ::Algorithm::Fenics::C1Operator<ResidualForm,JacobianForm> >( F , J , bcs , domain , range );
    }
  }
}

#endif // ALGORITHM_ADAPTER_FENICS_C1OPERATOR_HH
