#include "linearizedOperator.hh"

#include "abstractC1Operator.hh"
#include "Interface/abstractFunctionSpaceElement.hh"

namespace Algorithm
{
  LinearizedOperator::LinearizedOperator(const AbstractC1Operator &A)
    : AbstractLinearOperator(A.getDomain(),A.getRange()), A_(A)
  {}

  LinearizedOperator::LinearizedOperator(const AbstractC1Operator &A, std::shared_ptr<AbstractLinearSolver> solver)
    : AbstractLinearOperator(A.getDomain(),A.getRange()), A_(A), solver_(solver)
  {}

  std::unique_ptr<AbstractFunctionSpaceElement> LinearizedOperator::operator ()(const AbstractFunctionSpaceElement& dx) const
  {
    return A_.d1(dx);
  }

  std::shared_ptr<AbstractLinearSolver> LinearizedOperator::getSolver() const
  {
    return solver_;
  }

  void LinearizedOperator::setSolver(std::shared_ptr<AbstractLinearSolver> solver)
  {
    solver_ = solver;
  }

  LinearizedOperator* LinearizedOperator::cloneImpl() const
  {
    return new LinearizedOperator(A_,solver_);
  }
}
