#pragma once

namespace Spacy
{
  /// @cond
  class Vector;
  /// @endcond

  /** @addtogroup ScalarGroup @{ */
  namespace Scalar
  {
    class LinearSolver
    {
    public:
      explicit LinearSolver(double value);

      ::Spacy::Vector operator()(const ::Spacy::Vector& y) const;

    private:
      double value_;
    };
  }
  /** @} */
}
