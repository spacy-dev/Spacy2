#include "linearizedOperator.hh"

#include "boost/type_erasure/is_empty.hpp"

#include <utility>

namespace Spacy
{
  LinearizedOperator::LinearizedOperator(C1Operator A, Vector x)
    : OperatorBase(A.domain(),A.range()),
      A_(std::move(A)), x_(std::move(x))
  {}

 LinearizedOperator::LinearizedOperator(C1Operator A, Vector x, LinearSolver solver)
   : OperatorBase(A.domain(),A.range()),
     A_(std::move(A)),
     x_(std::move(x)),
     solver_(std::move(solver))
 {}

  Vector LinearizedOperator::operator ()(const Vector& dx) const
  {
    return A_.d1(x_,dx);
  }

  const LinearSolver& LinearizedOperator::solver() const
  {
    if( is_empty(solver_) ) throw std::runtime_error("Trying to access solver that has not been set in LinearizedOperator.");
    return solver_;
  }

//    std::unique_ptr<AbstractLinearSolver> LinearizedOperator::adjointSolver() const
//    {
//      return A_->makeAdjointSolver();
//    }
}
