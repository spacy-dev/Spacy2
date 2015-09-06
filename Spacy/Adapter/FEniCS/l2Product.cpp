#include "l2Product.hh"

#include "vector.hh"
#include "Spacy/Util/cast.hh"
#include "Spacy/Util/Exceptions/incompatibleSpaceException.hh"

#include "Spacy/Spaces/RealSpace/real.hh"
#include "Spacy/vectorSpace.hh"

namespace Spacy
{
  namespace FEniCS
  {
    Real l2Product::operator()(const ::Spacy::Vector& x, const ::Spacy::Vector& y) const
    {
      checkSpaceCompatibility(x.space(),y.space());
      return cast_ref<Vector>(x).impl().inner( cast_ref<Vector>(y).impl() );
    }

  }
}
