#pragma once

#include <Spacy/vectorSpace.hh>

namespace Spacy
{
  namespace Scalar
  {
    /**
     * @ingroup ScalarGroup
     * @brief Construct (Hilbert) space of real numbers.
     * @return ::Spacy::makeHilbertSpace() "::Spacy::makeHilbertSpace( [](const Spacy* space){ return Vector{*space}; } , ScalarProduct{} )"
     */
    VectorSpace makeScalarSpace(bool defaultIndex = false);
  }

  /**
   * @brief Construct space of real numbers.
   * @return ::Spacy::Scalar::makeRealHilbertSpace()
   */
  VectorSpace makeScalarSpace(bool defaultIndex = false);

  namespace Space
  {
    /**
     * @brief Global space of real numbers with space index 0.
     *
     * This space is used by class Real if the underlying space is left unspecified.
     */
    static VectorSpace R = Scalar::makeScalarSpace(true);
  }
}
