#include "realSolver.hh"

#include "Util/Exceptions/invalidArgumentException.hh"
#include "Util/cast.hh"
#include "real.hh"

namespace Algorithm
{
  RealSolver::RealSolver(double value)
     : value_(value)
  {}

  Vector RealSolver::operator ()(const Vector& y) const
  {
    if( !isAny<Real>(y) ) throw InvalidArgumentException("RealSolver::operator()(const Vector&)");

    auto x = y;
    x *= 1./value_;
    return x;
  }
}
