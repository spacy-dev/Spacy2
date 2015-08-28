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
      if( x.space()->index() != y.space()->index() )
        throw IncompatibleSpaceException("Real::ScalarProduct",x.space()->index(),y.space()->index());
      if( !is<Vector>(x) || !is<Vector>(y) ) throw InvalidArgumentException("Real::ScalarProduct");

      return cast_ref<Vector>(x).impl() * cast_ref<Vector>(y).impl();
    }
  }
}
