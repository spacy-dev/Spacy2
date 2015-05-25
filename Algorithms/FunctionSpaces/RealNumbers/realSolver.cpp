#include "realSolver.hh"

#include "../../Util/invalidargumentexception.hh"
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

    return 1. / A_.d1(makeElement<Real>(1.,A_.getDomain().impl())).coefficient(0) * y;
  }
}
