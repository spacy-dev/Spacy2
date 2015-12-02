// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "dynamicOperator.hh"

namespace Spacy
{
  Vector DynamicOperator::operator()(const Vector& x) const
  {
    assert(base_);
    return base_->operator ()(x);
  }

  LinearOperator DynamicOperator::M() const
  {
    assert(base_);
    return base_->M();
  }

  const VectorSpace& DynamicOperator::domain() const
  {
    assert(base_);
    return base_->domain();
  }

  const VectorSpace& DynamicOperator::range() const
  {
    assert(base_);
    return base_->range();
  }

  DynamicOperator::operator bool() const
  {
    return base_;
  }


  Vector DynamicC1Operator::operator()(double t, const Vector& x) const
  {
    assert(base_);
    return base_->operator ()(t,x);
  }

  Vector DynamicC1Operator::d1(double t, const Vector& x, const Vector& dx) const
  {
    assert(base_);
    return base_->d1(t,x,dx);
  }

  LinearOperator DynamicC1Operator::linearization(double t, const Vector& x) const
  {
    assert(base_);
    return base_->linearization(t,x);
  }

  LinearOperator DynamicC1Operator::M() const
  {
    assert(base_);
    return base_->M();
  }

  const VectorSpace& DynamicC1Operator::domain() const
  {
    assert(base_);
    return base_->domain();
  }

  const VectorSpace& DynamicC1Operator::range() const
  {
    assert(base_);
    return base_->range();
  }

  DynamicC1Operator::operator bool() const
  {
    return base_;
  }



  Vector DynamicLinearOperator::operator()(double t, const Vector& x) const
  {
    assert(base_);
    return base_->operator ()(t,x);
  }

  std::function<Vector(const Vector&)> DynamicLinearOperator::solver() const
  {
    assert(base_);
    return base_->solver();
  }

  DynamicLinearOperator& DynamicLinearOperator::operator+=(const DynamicLinearOperator& y)
  {
    assert(base_);
    base_->add(y);
    return *this;
  }

  DynamicLinearOperator& DynamicLinearOperator::operator-=(const DynamicLinearOperator& y)
  {
    assert(base_);
    base_->subtract(y);
    return *this;
  }

  DynamicLinearOperator& DynamicLinearOperator::operator*=(double a)
  {
    assert(base_);
    base_->multiply(a);
    return *this;
  }

  DynamicLinearOperator DynamicLinearOperator::operator-() const
  {
    assert(base_);
    return base_->negate();
  }

  bool DynamicLinearOperator::operator==(const DynamicLinearOperator& y) const
  {
    if( !base_ && !y ) return true;
    if( !base_ && y ) return false;
    if( base_ && !y ) return false;
    return base_->operator ==(y);
  }

  const VectorSpace& DynamicLinearOperator::space() const
  {
    assert(base_);
    return base_->space();
  }

  const VectorSpace& DynamicLinearOperator::domain() const
  {
    assert(base_);
    return base_->domain();
  }

  const VectorSpace& DynamicLinearOperator::range() const
  {
    assert(base_);
    return base_->range();
  }

  DynamicLinearOperator::operator bool() const
  {
    return base_;
  }
}
