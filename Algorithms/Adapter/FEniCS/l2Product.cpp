#include "l2Product.hh"

#include "vector.hh"
#include "Algorithms/Util/cast.hh"
#include "Algorithms/Util/Exceptions/incompatibleSpaceException.hh"

namespace Algorithm
{
  namespace FEniCS
  {
    double l2Product::operator()(const ::Algorithm::Vector& x, const ::Algorithm::Vector& y) const
    {
      checkSpaceCompatibility(x,y);
      return cast_ref<Vector>(x).impl().inner( cast_ref<Vector>(y).impl() );
    }

  }
}
