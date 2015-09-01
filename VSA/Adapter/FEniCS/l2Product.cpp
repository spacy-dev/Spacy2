#include "l2Product.hh"

#include "vector.hh"
#include "VSA/Util/cast.hh"
#include "VSA/Util/Exceptions/incompatibleSpaceException.hh"

namespace VSA
{
  namespace FEniCS
  {
    double l2Product::operator()(const ::VSA::Vector& x, const ::VSA::Vector& y) const
    {
      checkSpaceCompatibility(x,y);
      return cast_ref<Vector>(x).impl().inner( cast_ref<Vector>(y).impl() );
    }

  }
}
