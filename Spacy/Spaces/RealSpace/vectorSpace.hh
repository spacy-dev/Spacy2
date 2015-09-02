#ifndef ALGORITHM_REAL_SPACE_CREATOR_HH
#define ALGORITHM_REAL_SPACE_CREATOR_HH

#include "Spacy/vectorSpace.hh"

namespace Spacy
{
  namespace Real
  {
    /**
     * @ingroup RealGroup
     * @brief Construct space of real numbers.
     * @return ::Spacy::makeHilbertSpace() "::Spacy::makeHilbertSpace( [](const Spacy* space){ return Vector{*space}; } , ScalarProduct{} )"
     */
    VectorSpace makeHilbertSpace();
  }
}

#endif // ALGORITHM_REAL_SPACE_CREATOR_HH
