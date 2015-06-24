#include "linearSolver.hh"

#include "Interface/abstractLinearSolver.hh"
#include "c1Operator.hh"
#include "linearOperator.hh"

namespace Algorithm
{
  LinearSolver::LinearSolver(const LinearOperator& A)
    : impl_( A.impl().getSolver() )
  {}

  LinearSolver::LinearSolver(std::shared_ptr<AbstractLinearSolver> impl)
    : impl_(impl)
  {}

  FunctionSpaceElement LinearSolver::operator ()(const FunctionSpaceElement& x) const
  {
    return FunctionSpaceElement( impl()(x.impl()) );
  }

  AbstractLinearSolver& LinearSolver::impl()
  {
    return *impl_;
  }

  const AbstractLinearSolver& LinearSolver::impl() const
  {
    return *impl_;
  }
}

