#include "linearSolver.hh"

#include "Interface/abstractLinearSolver.hh"
#include "functionSpaceElement.hh"

namespace Algorithm
{
  LinearSolver::LinearSolver(std::unique_ptr<Interface::AbstractLinearSolver>&& impl)
    : Operator( std::unique_ptr<Interface::AbstractOperator>( impl.release() ) )
  {}

  FunctionSpaceElement LinearSolver::operator ()(const FunctionSpaceElement& x) const
  {
    return FunctionSpaceElement( impl()(x.impl()) );
  }

  bool LinearSolver::encounteredNonconvexity() const
  {
    return !dynamic_cast<const Interface::AbstractLinearSolver&>(impl()).systemIsPositiveDefinite();
  }
}

