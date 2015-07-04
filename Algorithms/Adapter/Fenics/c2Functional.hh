#ifndef ALGORITHMS_ADAPTER_FENICS_C2FUNCTIONAL_HH
#define ALGORITHMS_ADAPTER_FENICS_C2FUNCTIONAL_HH

#include <memory>
#include <vector>

#include "Interface/Functional/abstractC2Functional.hh"

#include "util.hh"
#include "vector.hh"

namespace Algorithm
{
  namespace Fenics
  {
    template <class BilinearForm, class LinearForm>
    class C2Functional : public Interface::AbstractC2Functional
    {
    public:
      C2Functional(const BilinearForm& a, const LinearForm& L, const std::vector<const dolfin::DirichletBC*>& bcs, std::shared_ptr<Interface::AbstractBanachSpace> space)
        : Interface::AbstractC2Functional( space ),
          a_( a.function_space(0) , a.functionSpace(1) ),
          L_( L.function_space(0) ),
          bcs_( bcs )
      {
        copyCoefficients(a,a_);
        copyCoefficients(L,L_);
      }

      double d0(const Interface::AbstractFunctionSpaceElement& x) const final override
      {
        const auto& x_ = toVector(x);
        assemble(x_);
        auto Ax = clone(x_);
        A_->mult(*x_.impl().vector(), *Ax->impl().vector());

        return Ax->impl().vector()->inner( *x_.impl().vector() ) + b_->inner( *x_.impl().vector() );
      }

      double d1(const Interface::AbstractFunctionSpaceElement &x, const Interface::AbstractFunctionSpaceElement &dx) const final override
      {
        const auto& x_ = toVector(x);
        const auto& dx_ = toVector(dx);
        assemble(x_);
        auto Ax = clone(x_);
        A_->mult(*x_.impl().vector(), *Ax->impl().vector());

        return Ax->impl().vector()->inner( *dx_.impl().vector() ) + b_->inner( *dx_.impl().vector() );
      }

      std::unique_ptr<Interface::AbstractFunctionSpaceElement> d2(const Interface::AbstractFunctionSpaceElement &x, const Interface::AbstractFunctionSpaceElement &dx) const final override
      {
        const auto& x_ = toVector(x);
        const auto& dx_ = toVector(dx);

        assemble(x_);
        auto Ax = clone(x_);
        A_->mult(*dx_.impl().vector(), *Ax->impl().vector());

        return std::unique_ptr<Interface::AbstractFunctionSpaceElement>( Ax.release() );
      }

    private:
      void assemble(const Vector& x) const
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

      C2Functional* cloneImpl() const
      {
        return new C2Functional(a_,L_,bcs_,getSharedDomain());
      }

      mutable BilinearForm a_;
      mutable LinearForm L_;
      const std::vector<const dolfin::DirichletBC*>& bcs_;
      mutable std::shared_ptr<dolfin::GenericMatrix> A_;
      mutable std::shared_ptr<dolfin::GenericVector> b_;
    };
  }
}

#endif // ALGORITHMS_ADAPTER_FENICS_C2FUNCTIONAL_HH

