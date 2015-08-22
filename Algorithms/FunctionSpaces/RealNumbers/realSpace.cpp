#include "realSpace.hh"

#include "realProduct.hh"

#include "vector.hh"
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
    return VectorSpace{ RealSpace{} , RealProduct{} };
  }
}
