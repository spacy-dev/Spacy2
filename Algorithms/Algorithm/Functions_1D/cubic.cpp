#include "cubic.hh"

namespace Algorithm
{
  namespace Functions_1D
  {
    Cubic::Cubic(double a, double b, double c, double d) noexcept
      : a_(a), b_(b), c_(c), d_(d)
    {}

    double Cubic::operator()(double t) const noexcept
    {
      return a_ + t*(b_ + t*(c_ + d_*t));
    }
  }
}
