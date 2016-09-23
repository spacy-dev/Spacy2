#include "cubic.hh"

namespace Spacy
{
  Cubic::Cubic(Real a, Real b, Real c, Real d) noexcept
    : a_(a), b_(b), c_(c), d_(d)
  {}

  Real Cubic::operator()(Real t) const noexcept
  {
    return a_ + t*(b_ + t*(c_ + d_*t));
  }
}
