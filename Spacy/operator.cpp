#include "operator.hh"


#include "Util/Exceptions/invalidArgumentException.hh"
#include "linearSolver.hh"
#include "vector.hh"

#include <cassert>
#include <iostream>

namespace Spacy
{
  Vector Operator::operator()(const Vector& x) const
  {
    assert(base_);
    return base_->operator ()(x);
//    return apply_(x);
  }

  const VectorSpace& Operator::domain() const
  {
    assert(base_);
    return base_->domain();
//    return domain_();
  }

  const VectorSpace& Operator::range() const
  {
    assert(base_);
    return base_->range();
//    return range_();
  }

  Operator::operator bool() const
  {
    return base_;
//    return !is_empty(*this);
  }


  Vector C1Operator::operator()(const Vector& x) const
  {
    assert(base_);
    return base_->operator ()(x);
//    assert( apply_ );
//    return apply_(x);
  }

  Vector C1Operator::d1(const Vector& x, const Vector& dx) const
  {
    assert(base_);
    return base_->d1(x,dx);
//    assert( d1_ );
//    return d1_(x,dx);
  }

  LinearOperator C1Operator::linearization(const Vector& x) const
  {
    assert(base_);
    return base_->linearization(x);
//    assert( linearization_ );
//    return linearization_(x);
  }

  const VectorSpace& C1Operator::domain() const
  {
    assert(base_);
    return base_->domain();
//    assert( domain_ );
//    return domain_();
  }

  const VectorSpace& C1Operator::range() const
  {
    assert(base_);
    return base_->range();
//    assert( range_ );
//    return range_();
  }

  C1Operator::operator bool() const
  {
    return base_;
//    return !is_empty(*this);
  }


  Vector LinearOperator::operator()(const Vector& x) const
  {
    return base_->operator ()(x);
//    return apply_(x);
  }

  std::function<Vector(const Vector&)> LinearOperator::solver() const
  {
    return base_->solver();
//    return solver_();
  }

  LinearOperator& LinearOperator::operator+=(const LinearOperator& y)
  {
    base_->add(y);
    return *this;
//    return base_->operator +=(y);
//    return add_(y);
  }

  LinearOperator& LinearOperator::operator-=(const LinearOperator& y)
  {
    base_->subtract(y);
    return *this;
//    return base_->operator -=(y);
//    return subtract_(y);
  }

  LinearOperator& LinearOperator::operator*=(double a)
  {
    base_->multiply(a);
    return *this;
//    return base_->operator *=(a);
//    return multiply_(a);
  }

  LinearOperator LinearOperator::operator-() const
  {
    return base_->negate();
//    return base_->operator -();
//    return negate_();
  }

  bool LinearOperator::operator==(const LinearOperator& y) const
  {
    return base_->operator ==(y);
//    return compare_(y);
  }

//  Vector LinearOperator::operator()(const LinearOperator& y) const
//  {
//    return applyAsDual_(y);
//  }

  const VectorSpace* LinearOperator::space() const
  {
    return base_->space();
//    return space_();
  }

  const VectorSpace& LinearOperator::domain() const
  {
    return base_->domain();
//    return domain_();
  }

  const VectorSpace& LinearOperator::range() const
  {
    return base_->range();
//    return range_();
  }

  LinearOperator::operator bool() const
  {
    return base_;
    //return apply_ && solver_ && add_ && subtract_ && multiply_ && compare_ /*&& applyAsDual_*/ && space_ && domain_ && range_;
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
