#include "realProduct.hh"

#include "real.hh"
#include "Util/Exceptions/incompatibleSpaceException.hh"
#include "Util/Exceptions/invalidArgumentException.hh"
#include "Util/cast.hh"

namespace Algorithm
{

  double RealProduct::operator()(const Vector& x, const Vector& y) const
  {
    if( x.spaceIndex() != y.spaceIndex() ) throw IncompatibleSpaceException("RealProduct",x.spaceIndex(),y.spaceIndex());
    if( !isAny<Real>(x) || !isAny<Real>(y) ) throw InvalidArgumentException("RealProduct");

    return cast_ref<Real>(x).impl() * cast_ref<Real>(y).impl();
  }
}
