#include "vector.hh"

#include <cmath>

namespace Algorithm
{
  namespace Real
  {
    Vector::Vector(double x, const ::Algorithm::VectorSpace& space)
      : VectorBase<Vector,double>(space,x)
    {}

    Vector::Vector(const ::Algorithm::VectorSpace& space)
      : Vector(0.,space)
    {}

    Vector& Vector::operator=(double y)
    {
      impl() = y;
      return *this;
    }

    double Vector::operator()(const Vector& y) const
    {
      return impl() * y.impl();
    }

    Vector::operator double() const
    {
      return impl();
    }

    unsigned Vector::size() const
    {
      return 1;
    }
  }
}
