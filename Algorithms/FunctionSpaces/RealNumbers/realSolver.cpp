#include "realSolver.hh"

#include "Util/Exceptions/invalidArgumentException.hh"
#include "Util/castTo.hh"
#include "real.hh"

namespace Algorithm
{
  RealSolver::RealSolver(double value, VectorSpace* domain, VectorSpace* range)
    : AbstractLinearSolver(domain,range),
      value_(value)
  {}

  Vector RealSolver::operator ()(const Vector& y) const
  {
    if( !isAny<Real>(y) ) throw InvalidArgumentException("RealSolver::operator()(const Vector&)");

    auto x = y;
    x *= 1./value_;
    return x;
  }

  RealSolver* RealSolver::cloneImpl() const
  {
    return new RealSolver(value_,domain_ptr(),range_ptr());
  }
}
