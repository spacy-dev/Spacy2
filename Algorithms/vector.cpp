#include "vector.hh"

namespace Algorithm
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
    return x.scalarProduct(y);
  }

  double norm(const Vector& x)
  {
    return x.norm();
  }
}
