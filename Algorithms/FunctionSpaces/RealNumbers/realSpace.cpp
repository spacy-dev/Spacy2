#include "realSpace.hh"

#include "real.hh"
#include "realProduct.hh"

#include "vector.hh"
#include "vectorSpace.hh"

namespace Algorithm
{
  Vector RealSpace::element(const VectorSpace*space) const
  {
    return Vector( std::make_unique<Real>(*space) );
  }

  VectorSpace makeRealSpace()
  {
    return VectorSpace( RealSpace() , std::make_shared<RealProduct>() );
  }
}
