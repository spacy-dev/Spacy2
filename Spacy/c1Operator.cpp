// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "c1Operator.hh"

namespace Spacy
{
  Vector C1Operator::operator()(const Vector& x) const
  {
    assert(base_);
    return base_->operator ()(x);
  }

  Vector C1Operator::d1(const Vector& x, const Vector& dx) const
  {
    assert(base_);
    return base_->d1(x,dx);
  }

  LinearOperator C1Operator::linearization(const Vector& x) const
  {
    assert(base_);
    return base_->linearization(x);
  }

  const VectorSpace& C1Operator::domain() const
  {
    assert(base_);
    return base_->domain();
  }

  const VectorSpace& C1Operator::range() const
  {
    assert(base_);
    return base_->range();
  }

  C1Operator::operator bool() const
  {
    return base_;
  }
}
