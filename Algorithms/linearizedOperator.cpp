#include "linearizedOperator.hh"

#include <utility>

namespace Algorithm
{
  LinearizedOperator::LinearizedOperator(C1Operator A, const Vector& x)
    : OperatorBase(A.domain_ptr(),A.range_ptr()),
      A_(std::move(A)), x_(x)
  {}

  Vector LinearizedOperator::operator ()(const Vector& dx) const
  {
    return A_.d1(x_,dx);
  }

  LinearSolver LinearizedOperator::solver() const
  {
    return A_.solver();
  }

//    std::unique_ptr<AbstractLinearSolver> LinearizedOperator::adjointSolver() const
//    {
//      return A_->makeAdjointSolver();
//    }
}
