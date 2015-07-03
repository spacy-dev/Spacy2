#ifndef FENICS_C1OPERATOR_HH
#define FENICS_C1OPERATOR_HH

#include <memory>
#include <vector>

#include <dolfin.h>

#include "Interface/Operator/abstractC1Operator.hh"
#include "Interface/Operator/linearizedOperator.hh"

#include "fenics_functionSpaceElement.hh"
#include "fenics_LUSolver.hh"

#include "banachSpace.hh"
#include "c1Operator.hh"

namespace Algorithm
{
  template <class ResidualForm, class JacobianForm>
  class Fenics_C1Operator : public AbstractC1Operator
  {
  public:
    explicit Fenics_C1Operator(std::shared_ptr<AbstractBanachSpace> space)
      : AbstractC1Operator(space,space), F_(nullptr), J_(nullptr)
    {}

    Fenics_C1Operator(ResidualForm& F, JacobianForm& J, const std::vector<const dolfin::DirichletBC*>& bcs, std::shared_ptr<AbstractBanachSpace> space)
      : AbstractC1Operator(space,space), F_(&F), J_(&J), bcs_(bcs)
    {}

    Fenics_C1Operator(ResidualForm& F, JacobianForm& J, const std::vector<const dolfin::DirichletBC*>& bcs, const BanachSpace& space)
      : Fenics_C1Operator(F,J,bcs,space.sharedImpl())
    {}

    std::unique_ptr<AbstractFunctionSpaceElement> operator()(const AbstractFunctionSpaceElement& x) const final override
    {
      const auto& x_ = fenics_Vector(x);
      assembleF(x_);
      auto y = clone(x_);
      *y->impl().vector() = *b_;

      return std::unique_ptr<AbstractFunctionSpaceElement>( y.release() );
    }

    std::unique_ptr<AbstractFunctionSpaceElement> d1(const AbstractFunctionSpaceElement &x, const AbstractFunctionSpaceElement &dx) const final override
    {
      const auto& x_ = fenics_Vector(x);
      const auto& dx_ = fenics_Vector(dx);

      assembleJ(x_);
      auto y = clone(x_);
      A_->mult(*dx_.impl().vector(), *y->impl().vector());

      return std::unique_ptr<AbstractFunctionSpaceElement>( y.release() );
    }

  private:
    void assembleF(const Fenics_Vector& x) const
    {
      if( F_ == nullptr ) return;

      if( oldX_F != nullptr && oldX_F->equals(x) ) return;
      oldX_F = clone(x);

      F_->u = x.impl();
      b_ = x.impl().vector()->factory().create_vector();

      // Assemble right-hand side
      dolfin::Assembler assembler;
      assembler.assemble(*b_, *F_);

      // Apply boundary conditions
      for(const auto& bc : bcs_)
        bc->apply( *b_ , *x.impl().vector() );
    }

    void assembleJ(const Fenics_Vector& x) const
    {
      if( J_ == nullptr ) return;

      if( oldX_J != nullptr && oldX_J->equals(x) ) return;
      oldX_J = clone(x);

      J_->u = x.impl();
      A_ = x.impl().vector()->factory().create_matrix();

      // Assemble right-hand side
      dolfin::Assembler assembler;
      assembler.assemble(*A_, *J_);

      // Apply boundary conditions
      for(const auto& bc : bcs_)
        bc->apply( *A_ );

      solver_ = std::make_shared<Fenics_LUSolver>( *A_ );
    }

    Fenics_C1Operator* cloneImpl() const
    {
      return new Fenics_C1Operator(getSharedDomain());
    }

    LinearizedOperator makeLinearization(const AbstractFunctionSpaceElement& x) const
    {
      assembleF(fenics_Vector(x));
      assembleJ(fenics_Vector(x));
      return LinearizedOperator(clone(*this),x,solver_);
    }

    mutable ResidualForm* F_;
    mutable JacobianForm* J_;
    std::vector<const dolfin::DirichletBC*> bcs_;
    mutable std::shared_ptr<dolfin::GenericMatrix> A_ = nullptr;
    mutable std::shared_ptr<dolfin::GenericVector> b_ = nullptr;
    mutable std::unique_ptr<Fenics_Vector> oldX_F, oldX_J = nullptr;
  };

  template <class ResidualForm, class JacobianForm>
  auto make_Fenics_C1Operator(ResidualForm& F, JacobianForm& J, const std::vector<const dolfin::DirichletBC*>& bcs, const BanachSpace& space)
  {
    return makeC1Operator< Fenics_C1Operator<ResidualForm,JacobianForm> >( F , J , bcs , space );
  }
}

#endif // FENICS_C1OPERATOR_HH
