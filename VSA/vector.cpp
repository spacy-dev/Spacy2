#include "vector.hh"

#include "VSA/Util/cast.hh"

namespace VSA
{
  // free functions
  Vector operator+(Vector x, const Vector& y)
  {
    return x += y;
  }

  Vector operator-(Vector x, const Vector& y)
  {
    return x -= y;
  }

  double operator*(const Vector& x, const Vector& y)
  {
    return x.space()->scalarProduct()(x,y);
  }

  double norm(const Vector& x)
  {
    return x.space()->norm()(x);
  }
}
