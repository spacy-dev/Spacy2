#include "linearSolver.hh"

#include "Interface/abstractLinearSolver.hh"
#include "c1Operator.hh"
#include "functionSpaceElement.hh"
#include "linearOperator.hh"

namespace Algorithm
{
  LinearSolver::LinearSolver(const LinearOperator& A)
    : SharedImpl<AbstractLinearSolver>( A.impl().getSolver() )
  {}

  LinearSolver::LinearSolver(std::shared_ptr<AbstractLinearSolver> impl)
    : SharedImpl<AbstractLinearSolver>(impl)
  {}

  FunctionSpaceElement LinearSolver::operator ()(const FunctionSpaceElement& x) const
  {
    return FunctionSpaceElement( impl()(x.impl()) );
  }
}

