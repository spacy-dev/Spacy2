#ifndef ALGORITHMS_ADAPTER_FENICS_C2FUNCTIONAL_HH
#define ALGORITHMS_ADAPTER_FENICS_C2FUNCTIONAL_HH

#include <memory>
#include <vector>

#include "Interface/Functional/abstractC2Functional.hh"

#include "fenics_functionSpaceElement.hh"

namespace Algorithm
{
  class Fenics_C2Functional : public AbstractC2Functional
  {
  public:
    Fenics_C2Functional(const dolfin::Form& a, const dolfin::Form& L, const std::vector<const dolfin::DirichletBC*>& bcs, const AbstractBanachSpace& space)
      : AbstractC2Functional(space), a_(a), L_(L), bcs_(bcs)
    {}

    double d0(const AbstractFunctionSpaceElement& x) const final override
    {
      const auto& x_ = fenics_Vector(x);
      assemble(x_);
      auto Ax = clone(x_);
      A_->mult(*x_.impl().vector(), *Ax->impl().vector());

      return Ax->impl().vector()->inner( *x_.impl().vector() ) + b_->inner( *x_.impl().vector() );
    }

    double d1(const AbstractFunctionSpaceElement &x, const AbstractFunctionSpaceElement &dx) const final override
    {
      const auto& x_ = fenics_Vector(x);
      const auto& dx_ = fenics_Vector(dx);
      assemble(x_);
      auto Ax = clone(x_);
      A_->mult(*x_.impl().vector(), *Ax->impl().vector());

      return Ax->impl().vector()->inner( *dx_.impl().vector() ) + b_->inner( *dx_.impl().vector() );
    }

    std::unique_ptr<AbstractFunctionSpaceElement> d2(const AbstractFunctionSpaceElement &x, const AbstractFunctionSpaceElement &dx) const final override
    {
      const auto& x_ = fenics_Vector(x);
      const auto& dx_ = fenics_Vector(dx);

      assemble(x_);
      auto Ax = clone(x_);
      A_->mult(*dx_.impl().vector(), *Ax->impl().vector());

      return std::unique_ptr<AbstractFunctionSpaceElement>( Ax.release() );
    }

  private:
    void assemble(const Fenics_Vector& x) const
    {

      A_ = x.impl().vector()->factory().create_matrix();
      b_ = x.impl().vector()->factory().create_vector();

      dolfin::assemble(*A_,a_);
      if( L_.ufc_form() )
        dolfin::assemble(*b_,L_);
      else
        A_->init_vector(*b_,0);

      for( auto& bc : bcs_) bc->apply(*A_,*b_);
    }

    Fenics_C2Functional* cloneImpl() const
    {
      return new Fenics_C2Functional(a_,L_,bcs_,getDomain());
    }

    const dolfin::Form& a_;
    const dolfin::Form& L_;
    const std::vector<const dolfin::DirichletBC*>& bcs_;
    mutable std::shared_ptr<dolfin::GenericMatrix> A_;
    mutable std::shared_ptr<dolfin::GenericVector> b_;
  };
}

#endif // ALGORITHMS_ADAPTER_FENICS_C2FUNCTIONAL_HH

