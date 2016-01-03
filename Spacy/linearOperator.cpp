// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "linearOperator.hh"

#include "Spacy/Util/Exceptions/invalidArgumentException.hh"

#include "linearSolver.hh"
#include "c1Operator.hh"

namespace Spacy
{
  Vector LinearOperator::operator()(const Vector& x) const
  {
    assert(base_);
    return base_->operator()(x);
  }

  Real LinearOperator::operator()(const LinearOperator& y) const
  {
    assert(base_);
    return base_->operator()(y);
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
  
  const VectorSpace& LinearOperator::space() const
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
    throw Exception::InvalidArgument("operator^ for LinearOperator only defined for exponent: k = -1.");
  }
  
  LinearSolver operator^(LinearOperator&& A, int k)
  {
    if( k == -1 ) return std::move(A.solver());
    throw Exception::InvalidArgument("operator^ for LinearOperator only defined for exponent: k = -1.");
  }
  
  LinearOperator d1(const C1Operator& A, const Vector& x)
  {
    return A.linearization(x);
  }
  
  LinearOperator& axpy(LinearOperator& A, double a, LinearOperator B)
  {
    return A += ( B*= a );
  }
}
