#ifndef ALGORITHM_REAL_SPACE_CREATOR_HH
#define ALGORITHM_REAL_SPACE_CREATOR_HH

#include "VSA/vectorSpace.hh"

namespace VSA
{
  namespace Real
  {
    /**
     * @ingroup RealGroup
     * @brief Construct space of real numbers.
     * @return ::VSA::makeHilbertSpace() "::VSA::makeHilbertSpace( [](const VSA* space){ return Vector{*space}; } , ScalarProduct{} )"
     */
    VectorSpace makeHilbertSpace();
  }
}

#endif // ALGORITHM_REAL_SPACE_CREATOR_HH
