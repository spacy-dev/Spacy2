#ifndef SPACY_ADAPTER_SCALAR_LINEAR_OPERATOR_CREATOR_HH
#define SPACY_ADAPTER_SCALAR_LINEAR_OPERATOR_CREATOR_HH

namespace Spacy
{
  /// @cond
  class VectorSpace;
  /// @endcond

  /** @addtogroup ScalarAdapterGroup
   * @{
   */
  namespace Scalar
  {
    /// @cond
    class LinearOperator;
    /// @endcond

    class LinearOperatorCreator
    {
    public:
      LinearOperatorCreator();

      LinearOperator operator()(const VectorSpace* space) const;

      const VectorSpace& domain() const;

      const VectorSpace& range() const;
    };
  }
  /** @} */
}

#endif // SPACY_ADAPTER_SCALAR_LINEAR_OPERATOR_CREATOR_HH
