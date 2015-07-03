#include "realSolver.hh"

#include "Util/Exceptions/invalidArgumentException.hh"
#include "real.hh"
#include <iostream>
namespace Algorithm
{
  RealSolver::RealSolver(double value)
    : value_(value)
  {}

  std::unique_ptr<AbstractFunctionSpaceElement> RealSolver::operator ()(const AbstractFunctionSpaceElement& y) const
  {
    if(  dynamic_cast<const Real*>(&y) == nullptr ) throw InvalidArgumentException("RealSolver::operator()(const FunctionSpaceElement&)");

    auto x = Algorithm::clone(y);
    *x *= 1./value_;
    return x;
  }
}
