#ifndef ALGORITHM_ADAPTER_KASKADE_DIRECT_SOLVER_HH
#define ALGORITHM_ADAPTER_KASKADE_DIRECT_SOLVER_HH

#include "Interface/abstractFunctionSpace.hh"
#include "Interface/abstractLinearSolver.hh"
#include "Interface/abstractFunctionSpaceElement.hh"
#include "Util/Mixins/impl.hh"
#include "Util/castTo.hh"

#include "vector.hh"

namespace Algorithm
{
  namespace Kaskade
  {
    template <class Description, class Domain, class Range>
    class DirectSolver :
        public Interface::AbstractLinearSolver ,
        public Mixin::MutableImpl< ::Kaskade::InverseLinearOperator< ::Kaskade::DirectSolver<Domain,Range> > >
    {
      using Spaces = typename Description::Spaces;
      using CoefficientVector = typename Description::template CoefficientVectorRepresentation<>::type;
    public:
      template <class KaskadeOperator>
      DirectSolver(const KaskadeOperator& A, const Spaces& spaces,
               std::shared_ptr<Interface::AbstractFunctionSpace> domain , std::shared_ptr<Interface::AbstractFunctionSpace> range)
        : Interface::AbstractLinearSolver(domain,range),
          Mixin::MutableImpl< ::Kaskade::InverseLinearOperator< ::Kaskade::DirectSolver<Domain,Range> > >( ::Kaskade::directInverseOperator(A, DirectType::UMFPACK3264) ),
          spaces_(spaces)
      {}


      std::unique_ptr<Interface::AbstractFunctionSpaceElement> operator()(const Interface::AbstractFunctionSpaceElement& x) const final override
      {
        CoefficientVector y_(Description::template CoefficientVectorRepresentation<>::init(spaces_));
        CoefficientVector x_(Description::template CoefficientVectorRepresentation<>::init(spaces_));
        copyToCoefficientVector<Description>(x,x_);
//        auto& y_ = castTo< Vector<Description> >(*y);

        this->impl().apply( /*castTo< Vector<Description> >(x).impl()*/x_ , y_ );

        auto y = range().element();
        copyFromCoefficientVector<Description>(y_,*y);

        return std::move(y);
      }

    private:
      DirectSolver* cloneImpl() const
      {
        return new DirectSolver(*this);
      }

      Spaces spaces_;
    };
  }
}

#endif // ALGORITHM_ADAPTER_KASKADE_DIRECT_SOLVER_HH
