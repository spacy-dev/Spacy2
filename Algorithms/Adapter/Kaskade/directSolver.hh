#ifndef ALGORITHM_ADAPTER_KASKADE_DIRECT_SOLVER_HH
#define ALGORITHM_ADAPTER_KASKADE_DIRECT_SOLVER_HH

#include "Interface/abstractBanachSpace.hh"
#include "Interface/abstractLinearSolver.hh"
#include "Interface/abstractFunctionSpaceElement.hh"
#include "Util/Mixins/impl.hh"

namespace Algorithm
{
  namespace Kaskade
  {
    template <class Description, class Domain, class Range>
    class DirectSolver :
        public Interface::AbstractLinearSolver ,
        public Mixin::MutableImpl< ::Kaskade::InverseLinearOperator< ::Kaskade::DirectSolver<Domain,Range> > >
    {
    public:
      template <class KaskadeOperator>
      DirectSolver(const KaskadeOperator& A,
               std::shared_ptr<Interface::AbstractBanachSpace> domain , std::shared_ptr<Interface::AbstractBanachSpace> range)
        : Interface::AbstractLinearSolver(domain,range),
          Mixin::MutableImpl< ::Kaskade::InverseLinearOperator< ::Kaskade::DirectSolver<Domain,Range> > >( ::Kaskade::directInverseOperator(A) )
      {}


      std::unique_ptr<Interface::AbstractFunctionSpaceElement> operator()(const Interface::AbstractFunctionSpaceElement& x) const final override
      {
        auto y = range().element();

        auto& y_ = castTo< Vector<Description> >(*y);

        this->impl().apply( castTo< Vector<Description> >(x).impl() , y_.impl() );

        return std::move(y);
      }

    private:
      DirectSolver* cloneImpl() const
      {
        return new DirectSolver(*this);
      }
    };
  }
}

#endif // ALGORITHM_ADAPTER_KASKADE_DIRECT_SOLVER_HH
