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


  Vector LinearOperator::operator()(const Vector& x) const
  {
    assert(base_);
    return base_->operator ()(x);
  }

  std::function<Vector(const Vector&)> LinearOperator::solver() const
  {
    assert(base_);
    return base_->solver();
  }

  LinearOperator& LinearOperator::operator+=(const LinearOperator& y)
  {
    assert(base_);
    base_->add(y);
    return *this;
  }

  LinearOperator& LinearOperator::operator-=(const LinearOperator& y)
  {
    assert(base_);
    base_->subtract(y);
    return *this;
  }

  LinearOperator& LinearOperator::operator*=(double a)
  {
    assert(base_);
    base_->multiply(a);
    return *this;
  }

  LinearOperator LinearOperator::operator-() const
  {
    assert(base_);
    return base_->negate();
  }

  bool LinearOperator::operator==(const LinearOperator& y) const
  {
    if( !base_ && !y ) return true;
    if( !base_ && y ) return false;
    if( base_ && !y ) return false;
    return base_->operator ==(y);
  }

  const VectorSpace* LinearOperator::space() const
  {
    assert(base_);
    return base_->space();
  }

  const VectorSpace& LinearOperator::domain() const
  {
    assert(base_);
    return base_->domain();
  }

  const VectorSpace& LinearOperator::range() const
  {
    assert(base_);
    return base_->range();
  }

  LinearOperator::operator bool() const
  {
    return base_;
  }



  LinearSolver operator^(const LinearOperator& A, int k)
  {
    if( k == -1 ) return A.solver();
    throw InvalidArgumentException("operator^ for LinearOperator only defined for exponent: k = -1.");
  }

  LinearSolver operator^(LinearOperator&& A, int k)
  {
    if( k == -1 ) return std::move(A.solver());
    throw InvalidArgumentException("operator^ for LinearOperator only defined for exponent: k = -1.");
  }

  LinearOperator d1(const C1Operator& A, const Vector& x)
  {
    return A.linearization(x);
  }
}
