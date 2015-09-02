#include "l2Product.hh"

#include "vector.hh"
#include "Spacy/Util/cast.hh"
#include "Spacy/Util/Exceptions/incompatibleSpaceException.hh"

namespace Spacy
{
  namespace FEniCS
  {
    double l2Product::operator()(const ::Spacy::Vector& x, const ::Spacy::Vector& y) const
    {
      checkSpaceCompatibility(x,y);
      return cast_ref<Vector>(x).impl().inner( cast_ref<Vector>(y).impl() );
    }

  }
}
