#include "realSolver.hh"

#include "../../Util/invalidargumentexception.hh"
#include "../../Interface/abstractOperator.hh"
#include "../../operator.hh"
#include "real.hh"

namespace Algorithm
{
  RealSolver::RealSolver(const Operator &A)
    : A_(A)
  {}

  FunctionSpaceElement RealSolver::operator ()(const FunctionSpaceElement& y) const
  {
    if(  dynamic_cast<const Real*>(&y.impl()) == nullptr ) throw InvalidArgumentException("RealSolver::operator()(const FunctionSpaceElement&)");

    auto dx = A_.impl().getDomain().element();
    dx->coefficient(0) = 1.;

    return 1. / A_.impl()(*dx)->coefficient(0) * y;
  }
}
