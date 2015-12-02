// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "operator.hh"

#include "Util/Exceptions/invalidArgumentException.hh"
#include "linearSolver.hh"
#include "vector.hh"

#include <cassert>

namespace Spacy
{
  Vector Operator::operator()(const Vector& x) const
  {
    assert(base_);
    return base_->operator ()(x);
  }

  const VectorSpace& Operator::domain() const
  {
    assert(base_);
    return base_->domain();
  }

  const VectorSpace& Operator::range() const
  {
    assert(base_);
    return base_->range();
  }

  Operator::operator bool() const
  {
    return base_;
  }
}
