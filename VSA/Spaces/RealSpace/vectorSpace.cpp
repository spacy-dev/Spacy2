#include "vectorSpace.hh"

#include "VSA/Spaces/RealSpace/vector.hh"
#include "VSA/Spaces/RealSpace/scalarProduct.hh"

VSA::VectorSpace VSA::Real::makeHilbertSpace()
{
  return ::VSA::makeHilbertSpace( [](const VectorSpace* space){ return Vector{*space}; } ,
                                        ScalarProduct{} );
}
