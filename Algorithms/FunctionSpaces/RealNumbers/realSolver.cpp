#include "realSolver.hh"

#include "Util/Exceptions/invalidArgumentException.hh"
#include "real.hh"
#include <iostream>
namespace Algorithm
{
  RealSolver::RealSolver(double value, VectorSpace* domain, VectorSpace* range)
    : AbstractLinearSolver(domain,range),
      value_(value)
  {}

  std::unique_ptr<Interface::AbstractVector> RealSolver::operator ()(const Interface::AbstractVector& y) const
  {
    if(  dynamic_cast<const Real*>(&y) == nullptr ) throw InvalidArgumentException("RealSolver::operator()(const Vector&)");

    auto x = Algorithm::clone(y);
    *x *= 1./value_;
    return x;
  }

  RealSolver* RealSolver::cloneImpl() const
  {
    std::cout << "cloning real solver" << std::endl;
    return new RealSolver(value_,domain_ptr(),range_ptr());
  }
}
