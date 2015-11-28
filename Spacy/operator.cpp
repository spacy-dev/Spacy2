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
    return apply_(x);
  }

  const VectorSpace& Operator::domain() const
  {
    return domain_();
  }

  const VectorSpace& Operator::range() const
  {
    return range_();
  }

  Operator::operator bool() const
  {
    return apply_ && domain_ && range_;
  }


  Vector C1Operator::operator()(const Vector& x) const
  {
    assert( apply_ );
    return apply_(x);
//    return base_.impl()(x);
  }

  Vector C1Operator::d1(const Vector& x, const Vector& dx) const
  {
    assert( d1_ );
    return d1_(x,dx);
//    return base_.impl().d1(x,dx);
  }

  LinearOperator C1Operator::linearization(const Vector& x) const
  {
    assert( linearization_ );
    if( !linearization_ ) std::cout << "grr" << std::endl;
    std::cout << "access linearization" << std::endl;
    return linearization_(x);
//    return base_.impl().linearization(x);
  }

  const VectorSpace& C1Operator::domain() const
  {
    assert( domain_ );
    return domain_();
//    return base_.impl().domain();
  }

  const VectorSpace& C1Operator::range() const
  {
    assert( range_ );
    return range_();
    //    return base_.impl().range();
  }

  C1Operator::operator bool() const
  {
    return apply_ && d1_ && linearization_ && domain_ && range_;
//    return !is_empty(base_);
  }


  Vector LinearOperator::operator()(const Vector& x) const
  {
    return apply_(x);
  }

  std::function<Vector(const Vector&)> LinearOperator::solver() const
  {
    return solver_();
  }

  LinearOperator& LinearOperator::operator+=(const LinearOperator& y)
  {
    return add_(y);
  }

  LinearOperator& LinearOperator::operator-=(const LinearOperator& y)
  {
    return subtract_(y);
  }

  LinearOperator& LinearOperator::operator*=(double a)
  {
    return multiply_(a);
  }

  LinearOperator LinearOperator::operator-() const
  {
    return negate_();
  }

  bool LinearOperator::operator==(const LinearOperator& y) const
  {
    return compare_(y);
  }

//  Vector LinearOperator::operator()(const LinearOperator& y) const
//  {
//    return applyAsDual_(y);
//  }

  const VectorSpace* LinearOperator::space() const
  {
    return space_();
  }

  const VectorSpace& LinearOperator::domain() const
  {
    return domain_();
  }

  const VectorSpace& LinearOperator::range() const
  {
    return range_();
  }

  LinearOperator::operator bool() const
  {
    return apply_ && solver_ && add_ && subtract_ && multiply_ && compare_ /*&& applyAsDual_*/ && space_ && domain_ && range_;
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
