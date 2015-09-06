#include "operator.hh"


#include "Util/Exceptions/invalidArgumentException.hh"
#include "linearSolver.hh"
#include "vector.hh"

namespace Spacy
{
  const LinearSolver& operator^(const LinearOperator& A, int k)
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
