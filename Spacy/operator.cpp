#include "operator.hh"


#include "Util/Exceptions/invalidArgumentException.hh"
#include "linearSolver.hh"
#include "vector.hh"

namespace Spacy
{
  Vector Operator::operator()(const Vector& x) const
  {
    return base_.impl()(x);
  }

  const VectorSpace& Operator::domain() const
  {
    return base_.impl().domain();
  }

  const VectorSpace& Operator::range() const
  {
    return base_.impl().range();
  }


  Vector C1Operator::operator()(const Vector& x) const
  {
    return base_.impl()(x);
  }

  Vector C1Operator::d1(const Vector& x, const Vector& dx) const
  {
    return base_.impl().d1(x,dx);
  }

  LinearOperator C1Operator::linearization(const Vector& x) const
  {
    return base_.impl().linearization(x);
  }

  const VectorSpace& C1Operator::domain() const
  {
    return base_.impl().domain();
  }

  const VectorSpace& C1Operator::range() const
  {
    return base_.impl().range();
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
