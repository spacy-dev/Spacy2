#include "linearSolver.hh"

#include "Interface/abstractLinearSolver.hh"
#include "c1Operator.hh"
#include "functionSpaceElement.hh"
#include "linearOperator.hh"

namespace Algorithm
{
  LinearSolver::LinearSolver(const LinearOperator& A)
    : Mixin::SharedImpl<Interface::AbstractLinearSolver>( A.getSolver().sharedImpl() )
  {}

  LinearSolver::LinearSolver(std::shared_ptr<Interface::AbstractLinearSolver> impl)
    : Mixin::SharedImpl<Interface::AbstractLinearSolver>(impl)
  {}

  FunctionSpaceElement LinearSolver::operator ()(const FunctionSpaceElement& x) const
  {
    return FunctionSpaceElement( impl()(x.impl()) );
  }
}

