#include "vectorSpace.hh"

#include "Spacy/Spaces/RealSpace/vector.hh"
#include "Spacy/Spaces/RealSpace/scalarProduct.hh"

Spacy::VectorSpace Spacy::Real::makeHilbertSpace()
{
  return ::Spacy::makeHilbertSpace( [](const VectorSpace* space){ return Vector{*space}; } ,
                                        ScalarProduct{} );
}
