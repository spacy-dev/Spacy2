#include "l2Product.hh"

#include "vector.hh"
#include "Util/cast.hh"
#include "Util/Exceptions/incompatibleSpaceException.hh"

namespace Algorithm
{
  namespace Fenics
  {
    double l2Product::operator()(const ::Algorithm::Vector& x, const ::Algorithm::Vector& y) const
    {
      if( x.space()->index() != y.space()->index() )
        throw IncompatibleSpaceException("Fenics::l2Product",x.space()->index(),y.space()->index());
      return cast_ref<Vector>(x).impl().inner( cast_ref<Vector>(y).impl() );
    }

  }
}
