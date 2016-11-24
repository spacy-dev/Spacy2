#include "l2Product.hh"

#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>
#include <Spacy/Spaces/ScalarSpace/real.hh>
#include <Spacy/Util/cast.hh>

#include "vector.hh"

namespace Spacy
{
  namespace FEniCS
  {
    Real l2Product::operator()(const ::Spacy::Vector& x, const ::Spacy::Vector& y) const
    {
      checkSpaceCompatibility(x.space(),y.space());
      return cast_ref<Vector>(x).get().inner( cast_ref<Vector>(y).get() );
    }
  }
}
