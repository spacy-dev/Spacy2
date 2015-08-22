#include "scalarProducts.hh"

#include "vector.hh"
#include "Util/castTo.hh"

namespace Algorithm
{
  namespace Fenics
  {
    double l2Product::operator()(const ::Algorithm::Vector& x, const ::Algorithm::Vector& y) const
    {
      return castAny<Vector>(x).impl().vector()->inner( *castAny<Vector>(y).impl().vector() );
    }

  }
}
