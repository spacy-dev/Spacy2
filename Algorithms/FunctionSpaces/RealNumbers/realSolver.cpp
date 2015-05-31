#include "realSolver.hh"

#include "../../Util/invalidargumentexception.hh"
#include "../../Interface/abstractC0Operator.hh"
#include "../../c0Operator.hh"
#include "real.hh"

namespace Algorithm
{
  RealSolver::RealSolver(const C0Operator &A)
    : A_(A)
  {}

  FunctionSpaceElement RealSolver::operator ()(const FunctionSpaceElement& y) const
  {
    if(  dynamic_cast<const Real*>(&y.impl()) == nullptr ) throw InvalidArgumentException("RealSolver::operator()(const FunctionSpaceElement&)");

    auto val = 0.;
    A_.impl().getMatrix(&val,&val);

    return 1. / val * y;
  }
}
