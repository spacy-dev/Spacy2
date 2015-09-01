#include "quadratic.hh"

namespace VSA
{
  namespace Functions_1D
  {
    Quadratic::Quadratic(double a, double b, double c) noexcept
      : a_(a), b_(b), c_(c)
    {}

    double Quadratic::operator()(double t) const noexcept
    {
      return a_ + t*b_ + t*t*c_;
    }
  }
}
