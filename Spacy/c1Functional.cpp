// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "c1Functional.hh"

#include "Spacy/vector.hh"
#include "Spacy/Spaces/RealSpace/real.hh"

#include <cassert>

namespace Spacy
{
  Real C1Functional::operator()(const Vector& x) const
  {
    assert(base_);
    return base_->operator ()(x);
  }

  Vector C1Functional::d1(const Vector& x) const
  {
    assert(base_);
    return base_->d1(x);
  }

  const VectorSpace& C1Functional::domain() const
  {
    assert(base_);
    return base_->domain();
  }

  C1Functional::operator bool() const
  {
    return base_;
  }
}
