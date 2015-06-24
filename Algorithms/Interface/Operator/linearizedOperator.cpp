#include "linearizedOperator.hh"

#include "abstractC1Operator.hh"
#include "Interface/Functional/abstractC2Functional.hh"
#include "Interface/abstractFunctionSpaceElement.hh"

namespace Algorithm
{
  LinearizedOperator::LinearizedOperator(const AbstractC1Operator &A, const AbstractFunctionSpaceElement& x)
    : AbstractLinearOperator(A.getDomain(),A.getRange()), A_(A), x_(x)
  {}

  LinearizedOperator::LinearizedOperator(const AbstractC1Operator &A, const AbstractFunctionSpaceElement& x, std::shared_ptr<AbstractLinearSolver> solver)
    : AbstractLinearOperator(A.getDomain(),A.getRange()), A_(A), x_(x), solver_(solver)
  {}

  std::unique_ptr<AbstractFunctionSpaceElement> LinearizedOperator::operator ()(const AbstractFunctionSpaceElement& dx) const
  {
    return A_.d1(x_,dx);
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
    return new LinearizedOperator(A_,x_,solver_);
  }
}
