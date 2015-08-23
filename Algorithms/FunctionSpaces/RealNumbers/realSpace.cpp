#include "realSpace.hh"

#include "real.hh"
#include "realProduct.hh"
#include "vectorSpace.hh"

namespace Algorithm
{
  Vector RealSpace::element(const VectorSpace* space) const
  {
    Vector v = Real{*space};
    return v;
  }

  VectorSpace makeRealSpace()
  {
    return makeHilbertSpace( RealSpace{} , RealProduct{} );
  }
}
