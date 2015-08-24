#include "scalarProducts.hh"

#include "vector.hh"
#include "Util/cast.hh"

namespace Algorithm
{
  namespace Fenics
  {
    double l2Product::operator()(const ::Algorithm::Vector& x, const ::Algorithm::Vector& y) const
    {
      return cast_ref<Vector>(x).impl().inner( cast_ref<Vector>(y).impl() );
    }

  }
}
