#include "quadratic.hh"

namespace Spacy
{
  Quadratic::Quadratic(Real a, Real b, Real c) noexcept
    : a_(a), b_(b), c_(c)
  {}

  Real Quadratic::operator()(Real t) const noexcept
  {
    return a_ + t*b_ + t*t*c_;
  }
}
