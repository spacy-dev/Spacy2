#include "scalarProduct.hh"

#include "Spacy/Spaces/RealSpace/real.hh"
#include "Spacy/vector.hh"

namespace Spacy
{
  namespace Generic
  {
    Spacy::Real EuclideanScalarProduct::operator()(const ::Spacy::Vector& x, const ::Spacy::Vector& y) const
    {
      checkSpaceCompatibility(x.space(),y.space());
      return x(y);
    }
  }
}

