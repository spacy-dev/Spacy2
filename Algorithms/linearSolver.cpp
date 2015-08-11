#include "linearSolver.hh"

#include "Interface/abstractLinearSolver.hh"
#include "vector.hh"

namespace Algorithm
{
  LinearSolver::LinearSolver(std::unique_ptr<Interface::AbstractLinearSolver>&& impl)
    : Operator( std::unique_ptr<Interface::AbstractOperator>( impl.release() ) )
  {}

  Vector LinearSolver::operator ()(const Vector& x) const
  {
    return Vector( impl()(x.impl()) );
  }

  bool LinearSolver::encounteredNonconvexity() const
  {
    return !dynamic_cast<const Interface::AbstractLinearSolver&>(impl()).systemIsPositiveDefinite();
  }
}

