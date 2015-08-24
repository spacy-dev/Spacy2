#include "linearizedOperator.hh"

#include <utility>

namespace Algorithm
{
  LinearizedOperator::LinearizedOperator(C1Operator A, const Vector& x)
    : OperatorBase(A.domain_ptr(),A.range_ptr()),
      A_(std::move(A)), x_(x)
  {}

 LinearizedOperator::LinearizedOperator(C1Operator A, const Vector& x, std::shared_ptr<LinearSolver> solver)
   : LinearizedOperator(A,x)
 {
    solver_ = solver;
 }

  Vector LinearizedOperator::operator ()(const Vector& dx) const
  {
    return A_.d1(x_,dx);
  }

  const LinearSolver& LinearizedOperator::solver() const
  {
    return *solver_;
//    return A_.solver();
  }

//    std::unique_ptr<AbstractLinearSolver> LinearizedOperator::adjointSolver() const
//    {
//      return A_->makeAdjointSolver();
//    }
}
