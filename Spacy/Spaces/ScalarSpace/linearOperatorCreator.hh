#pragma once

namespace Spacy
{
  /// @cond
  class VectorSpace;
  /// @endcond

  /** @addtogroup ScalarGroup @{ */
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
