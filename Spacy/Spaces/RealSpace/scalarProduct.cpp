#include "scalarProduct.hh"

#include "Spacy/Spaces/RealSpace/vector.hh"
#include "Spacy/Util/Exceptions/invalidArgumentException.hh"
#include "Spacy/Util/cast.hh"

namespace Spacy
{
  namespace Real
  {
    double ScalarProduct::operator()(const ::Spacy::Vector& x, const ::Spacy::Vector& y) const
    {
      checkSpaceCompatibility(x,y);
      if( !is<Vector>(x) || !is<Vector>(y) ) throw InvalidArgumentException("Real::ScalarProduct");

      return cast_ref<Vector>(x).impl() * cast_ref<Vector>(y).impl();
    }
  }
}
