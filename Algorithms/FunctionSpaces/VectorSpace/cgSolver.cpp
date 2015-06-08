#include "cgSolver.hh"

#include "operator.hh"
#include "Algorithm/ConjugateGradients/jacobipreconditioner.hh"

namespace Algorithm
{
  CGSolver::CGSolver(const Operator &A)
    : P_(jacobiPreconditioner(A)),
      cg(A,P_)
  {}

  CGSolver::CGSolver(const LinearOperator &A)
    : P_(jacobiPreconditioner(A)),
      cg(A,P_,true)
  {}

  std::unique_ptr<AbstractFunctionSpaceElement> CGSolver::operator ()(const AbstractFunctionSpaceElement& y) const
  {
    auto z = cg.solve( FunctionSpaceElement( P_.impl().getRange().element() ) ,
                     FunctionSpaceElement( clone(y) ) );
    return clone(z.impl());
  }
}
