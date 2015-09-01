#include "vector.hh"

#include "VSA/vector.hh"
#include <cmath>

namespace VSA
{
  namespace Real
  {
    Vector::Vector(double x, const VectorSpace& space)
      : VectorBase<Vector,double>(space,x)
    {}

    Vector::Vector(const VectorSpace& space)
      : Vector(0.,space)
    {}

    Vector& Vector::operator=(double y)
    {
      impl() = y;
      return *this;
    }

    double Vector::operator()(const Vector& y) const
    {
      checkDualPairing(*this,y);
      return impl() * y.impl();
    }

    Vector::operator double() const
    {
      return impl();
    }
  }
}
