#ifndef ALGORITHM_ADAPTER_KASKADE_DIRECT_SOLVER_HH
#define ALGORITHM_ADAPTER_KASKADE_DIRECT_SOLVER_HH

#include "linalg/direct.hh"

#include "vector.hh"
#include "Util/cast.hh"
#include "Util/Base/operatorBase.hh"

namespace Algorithm
{
  namespace Kaskade
  {
    template <class AnsatzVariableDescription, class TestVariableDescription>
    class DirectSolver :
        public OperatorBase
    {
      using Spaces = typename AnsatzVariableDescription::Spaces;
      using Domain = typename AnsatzVariableDescription::template CoefficientVectorRepresentation<>::type;
      using Range = typename TestVariableDescription::template CoefficientVectorRepresentation<>::type;
    public:
      template <class KaskadeOperator>
      DirectSolver(const KaskadeOperator& A, const Spaces& spaces,
               ::Algorithm::VectorSpace* domain , ::Algorithm::VectorSpace* range)
        : OperatorBase(domain,range),
          solver_( ::Kaskade::InverseLinearOperator< ::Kaskade::DirectSolver<Domain,Range> >( ::Kaskade::directInverseOperator(A, DirectType::UMFPACK3264) ) ) ,
          spaces_(spaces)
      {}


      ::Algorithm::Vector operator()(const ::Algorithm::Vector& x) const
      {
        Range y_(TestVariableDescription::template CoefficientVectorRepresentation<>::init(spaces_));
        Domain x_(AnsatzVariableDescription::template CoefficientVectorRepresentation<>::init(spaces_));
        copyToCoefficientVector<AnsatzVariableDescription>(x,x_);

        solver_.apply( x_ , y_ );

        auto y = range().element();
        copyFromCoefficientVector<TestVariableDescription>(y_,y);

        return y;
      }

    private:
      ::Kaskade::InverseLinearOperator< ::Kaskade::DirectSolver<Domain,Range> > solver_;
      Spaces spaces_;
    };
  }
}

#endif // ALGORITHM_ADAPTER_KASKADE_DIRECT_SOLVER_HH
