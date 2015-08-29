#ifndef ALGORITHM_REAL_SPACE_CREATOR_HH
#define ALGORITHM_REAL_SPACE_CREATOR_HH

#include "Algorithms/vectorSpace.hh"

namespace Algorithm
{
  namespace Real
  {
    /**
     * @ingroup RealGroup
     * @brief Construct space of real numbers.
     * @return ::Algorithm::makeHilbertSpace( [](const VectorSpace* space){ return Vector{*space}; } , ScalarProduct{} );
     */
    VectorSpace makeHilbertSpace();
  }
}

#endif // ALGORITHM_REAL_SPACE_CREATOR_HH
