#include "linearSolver.hh"

#include "c1Operator.hh"
#include "functionSpaceElement.hh"
#include "linearOperator.hh"

#include <utility>

namespace Algorithm
{
//  LinearSolver::LinearSolver(const LinearOperator& A)
//    : Operator( clone(A.solver() ) )
////    : Mixin::SharedImpl<Interface::AbstractLinearSolver>( A.solver() )
//  {}

  LinearSolver::LinearSolver(std::unique_ptr<Interface::AbstractLinearSolver>&& impl)
    : Operator( std::unique_ptr<Interface::AbstractOperator>( impl.release() ) )
//      : Mixin::SharedImpl<Interface::AbstractLinearSolver>(impl)
  {}

  FunctionSpaceElement LinearSolver::operator ()(const FunctionSpaceElement& x) const
  {
    return FunctionSpaceElement( impl()(x.impl()) );
  }

  bool LinearSolver::encounteredNonconvexity() const
  {
    return dynamic_cast<const Interface::AbstractLinearSolver&>(impl()).encounteredNonconvexity();
  }
}

