// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "c2Functional.hh"

#include "Spacy/vector.hh"
#include "Spacy/Spaces/RealSpace/real.hh"

#include <cassert>

namespace Spacy
{


  Real C2Functional::operator()(const Vector& x) const
  {
    assert(base_);
    return base_->operator ()(x);
  }

  Vector C2Functional::d1(const Vector& x) const
  {
    assert(base_);
    return base_->d1(x);
  }

  Vector C2Functional::d2(const Vector& x, const Vector& dx) const
  {
    assert(base_);
    return base_->d2(x,dx);
  }

  LinearOperator C2Functional::hessian(const Vector& x) const
  {
    assert(base_);
    return base_->hessian(x);
  }

  const VectorSpace& C2Functional::domain() const
  {
    assert(base_);
    return base_->domain();
  }

  C2Functional::operator bool() const
  {
    return base_;
  }


  Vector d1(const C2Functional& f, const Vector& x)
  {
    return f.d1(x);
  }

  LinearOperator d2(const C2Functional& f, const Vector& x)
  {
    return f.hessian(x);
  }
}
