#ifndef ALGORITHM_ADAPTER_KASKADE_DIRECT_SOLVER_HH
#define ALGORITHM_ADAPTER_KASKADE_DIRECT_SOLVER_HH

#include "vector.hh"
#include "Interface/abstractLinearSolver.hh"
#include "Util/Mixins/impl.hh"
#include "Util/castTo.hh"

#include "vector.hh"

#include "linalg/direct.hh"

namespace Algorithm
{
  namespace Kaskade
  {
    template <class AnsatzVariableDescription, class TestVariableDescription>
    class DirectSolver :
        public Interface::AbstractLinearSolver ,
        public Mixin::MutableImpl< ::Kaskade::InverseLinearOperator< ::Kaskade::DirectSolver<typename AnsatzVariableDescription::template CoefficientVectorRepresentation<>::type,typename TestVariableDescription::template CoefficientVectorRepresentation<>::type> > >
    {
      using Spaces = typename AnsatzVariableDescription::Spaces;
      using Domain = typename AnsatzVariableDescription::template CoefficientVectorRepresentation<>::type;
      using Range = typename TestVariableDescription::template CoefficientVectorRepresentation<>::type;
    public:
      template <class KaskadeOperator>
      DirectSolver(const KaskadeOperator& A, const Spaces& spaces,
               ::Algorithm::VectorSpace* domain , ::Algorithm::VectorSpace* range)
        : Interface::AbstractLinearSolver(domain,range),
          Mixin::MutableImpl< ::Kaskade::InverseLinearOperator< ::Kaskade::DirectSolver<Domain,Range> > >( ::Kaskade::directInverseOperator(A, DirectType::UMFPACK3264) ),
          spaces_(spaces)
      {}


      ::Algorithm::Vector operator()(const ::Algorithm::Vector& x) const final override
      {
        Range y_(TestVariableDescription::template CoefficientVectorRepresentation<>::init(spaces_));
        Domain x_(AnsatzVariableDescription::template CoefficientVectorRepresentation<>::init(spaces_));
        copyToCoefficientVector<AnsatzVariableDescription>(x,x_);

        this->impl().apply( /*castTo< Vector<Description> >(x).impl()*/x_ , y_ );

        auto y = range().element();
        copyFromCoefficientVector<TestVariableDescription>(y_,y);

        return y;
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
