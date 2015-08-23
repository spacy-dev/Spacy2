#include "scalarProduct.hh"

#include "vector.hh"
#include "Util/Exceptions/incompatibleSpaceException.hh"
#include "Util/Exceptions/invalidArgumentException.hh"
#include "Util/cast.hh"

namespace Algorithm
{
  namespace Real
  {
    double ScalarProduct::operator()(const ::Algorithm::Vector& x, const ::Algorithm::Vector& y) const
    {
      if( x.spaceIndex() != y.spaceIndex() ) throw IncompatibleSpaceException("Real::ScalarProduct",x.spaceIndex(),y.spaceIndex());
      if( !isAny<Vector>(x) || !isAny<Vector>(y) ) throw InvalidArgumentException("Real::ScalarProduct");

      return cast_ref<Vector>(x).impl() * cast_ref<Vector>(y).impl();
    }
  }
}
