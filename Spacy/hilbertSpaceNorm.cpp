// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "hilbertSpaceNorm.hh"

#include "Spacy/Spaces/RealSpace/real.hh"
#include "Spacy/vector.hh"

namespace Spacy
{
  HilbertSpaceNorm::HilbertSpaceNorm(ScalarProduct sp)
    : sp_(std::move(sp))
  {}

  Real HilbertSpaceNorm::operator()(const Vector& x) const
  {
    return sqrt(sp_(x,x));
  }
}
