#include "realSolver.hh"

#include "Util/invalidargumentexception.hh"
#include "Interface/abstractC0Operator.hh"
#include "c0Operator.hh"
#include "real.hh"
#include <iostream>
namespace Algorithm
{
  RealSolver::RealSolver(double value)
    : value_(value)
  {}

  FunctionSpaceElement RealSolver::operator ()(const FunctionSpaceElement& y) const
  {
    if(  dynamic_cast<const Real*>(&y.impl()) == nullptr ) throw InvalidArgumentException("RealSolver::operator()(const FunctionSpaceElement&)");

    return 1. / value_ * y;
  }
}
