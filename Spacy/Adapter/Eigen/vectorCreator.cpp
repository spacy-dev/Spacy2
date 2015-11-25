#include "vectorCreator.hh"

#include "scalarProduct.hh"

namespace Spacy
{
  namespace Rn
  {
    VectorSpace makeHilbertSpace(unsigned dim)
    {
      return ::Spacy::makeHilbertSpace( VectorCreator{dim} , EuclideanScalarProduct{} );
    }
  }
}
