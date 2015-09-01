#include "scalarProduct.hh"

#include "VSA/Spaces/RealSpace/vector.hh"
#include "VSA/Util/Exceptions/invalidArgumentException.hh"
#include "VSA/Util/cast.hh"

namespace VSA
{
  namespace Real
  {
    double ScalarProduct::operator()(const ::VSA::Vector& x, const ::VSA::Vector& y) const
    {
      checkSpaceCompatibility(x,y);
      if( !is<Vector>(x) || !is<Vector>(y) ) throw InvalidArgumentException("Real::ScalarProduct");

      return cast_ref<Vector>(x).impl() * cast_ref<Vector>(y).impl();
    }
  }
}
