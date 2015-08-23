#include "vectorSpace.hh"

#include "vector.hh"
#include "scalarProduct.hh"

::Algorithm::VectorSpace Algorithm::Real::makeHilbertSpace()
{
  return ::Algorithm::makeHilbertSpace( [](const VectorSpace* space){ return Vector{*space}; } ,
                                        ScalarProduct{} );
}
