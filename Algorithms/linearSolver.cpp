#include "linearSolver.hh"

#include "Interface/abstractLinearSolver.hh"
#include "c1Operator.hh"

namespace Algorithm
{
  LinearSolver::LinearSolver(const C1Operator& A)
    : impl_( A.getLinearization().getSolver().impl_ )
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

