#include "cgSolver.hh"

#include "../../Util/invalidargumentexception.hh"
#include "../../operator.hh"
#include "vectorSpaceElement.hh"
#include "../Algorithm/ConjugateGradients/jacobipreconditioner.hh"

namespace Algorithm
{
  CGSolver::CGSolver(const Operator &A)
    : P_(jacobi(A)),
      cg(A,P_)
  {}

  FunctionSpaceElement CGSolver::operator ()(const FunctionSpaceElement& x, const FunctionSpaceElement& y) const
  {
    //if(  dynamic_cast<const VectorSpaceElement*>(&y.impl()) == nullptr ) throw InvalidArgumentException("CGSolver::operator()(const FunctionSpaceElement&)");
    return cg.solve(x,y);
  }

  FunctionSpaceElement CGSolver::operator ()(const FunctionSpaceElement& y) const
  {
//    if(  dynamic_cast<const VectorSpaceElement*>(&y.impl()) == nullptr ) throw InvalidArgumentException("CGSolver::operator()(const FunctionSpaceElement&)");

    auto x = P_.getRange().element();

    return (*this)(x,y);
  }
}
