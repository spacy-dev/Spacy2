#ifndef SPACY_ADAPTER_KASKADE_OPERATOR_SPACE_HH
#define SPACY_ADAPTER_KASKADE_OPERATOR_SPACE_HH

#include "vector.hh"

namespace Spacy
{
  /// @cond
  class VectorSpace;
  /// @endcond

  namespace Kaskade
  {
    /// @cond
    template <class,class> class LinearOperator;
    /// @endcond

    template <class AnsatzVariableSetDescription, class TestVariableSetDescription>
    class LinearOperatorCreator
    {
      using Scalar = typename AnsatzVariableSetDescription::Scalar;
      using Domain = typename AnsatzVariableSetDescription::template CoefficientVectorRepresentation<>::type;
      using Range = typename TestVariableSetDescription::template CoefficientVectorRepresentation<>::type;
      using Matrix = ::Kaskade::MatrixAsTriplet<Scalar>;
      using OperatorImpl = ::Kaskade::MatrixRepresentedOperator<Matrix,Domain,Range>;

    public:
      LinearOperatorCreator(const VectorSpace& domain, const VectorSpace& range)
        : domain_(domain),
          range_(range)
      {}

      ::Spacy::Vector operator()(const VectorSpace* space)
      {
        return LinearOperator<AnsatzVariableSetDescription,TestVariableSetDescription>{ OperatorImpl{ Matrix{} } , *space };
      }

      const VectorSpace& domain() const
      {
        return domain_;
      }

      const VectorSpace& range() const
      {
        return range_;
      }

    private:
      const VectorSpace &domain_, &range_;
    };
  }
}

#endif // SPACY_ADAPTER_KASKADE_OPERATOR_SPACE_HH
