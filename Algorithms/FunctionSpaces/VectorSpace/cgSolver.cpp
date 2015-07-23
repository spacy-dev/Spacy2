//#include "cgSolver.hh"

//#include "operator.hh"
//#include "Algorithm/ConjugateGradients/jacobipreconditioner.hh"

//namespace Algorithm
//{
//  using Interface::AbstractFunctionSpaceElement;

//  CGSolver::CGSolver(const Operator &A)
//    : P_(jacobiPreconditioner(A)),
//      cg(A,P_)
//  {}

//  std::unique_ptr<AbstractFunctionSpaceElement> CGSolver::operator ()(const AbstractFunctionSpaceElement& y) const
//  {
//    auto z = cg.solve( FunctionSpaceElement( P_.impl().getRange().element() ) ,
//                     FunctionSpaceElement( clone(y) ) );
//    return clone(z.impl());
//  }
//}
