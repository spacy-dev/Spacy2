// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "quadratic.hh"

namespace Spacy
{
  namespace Functions_1D
  {
    Quadratic::Quadratic(Real a, Real b, Real c) noexcept
      : a_(a), b_(b), c_(c)
    {}

    Real Quadratic::operator()(Real t) const noexcept
    {
      return a_ + t*b_ + t*t*c_;
    }
  }
}
