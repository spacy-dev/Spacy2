#include "real.hh"

#include <cmath>

namespace Algorithm
{
  Real::Real(double x, const VectorSpace &space)
    : VectorBase<Real,double>(space,x)
  {}

  Real::Real(const VectorSpace& space)
    : Real(0.,space)
  {}

  Real& Real::operator=(double y)
  {
    impl() = y;
    return *this;
  }

  double Real::operator()(const Real& y) const
  {
    return impl() * y.impl();
  }

  Real::operator double() const
  {
    return impl();
  }

  unsigned Real::size() const
  {
    return 1;
  }

  bool Real::operator==(const Real& y) const
  {
    return std::fabs(impl()-y.impl()) < eps();
  }
}
