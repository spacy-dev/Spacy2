#include "scalarProduct.hh"

#include "Algorithms/VectorSpaces/RealSpace/vector.hh"
#include "Algorithms/Util/Exceptions/invalidArgumentException.hh"
#include "Algorithms/Util/cast.hh"

namespace Algorithm
{
  namespace Real
  {
    double ScalarProduct::operator()(const ::Algorithm::Vector& x, const ::Algorithm::Vector& y) const
    {
      checkSpaceCompatibility(x,y);
      if( !is<Vector>(x) || !is<Vector>(y) ) throw InvalidArgumentException("Real::ScalarProduct");

      return cast_ref<Vector>(x).impl() * cast_ref<Vector>(y).impl();
    }
  }
}
