#include "realSolver.hh"

#include "Interface/abstractBanachSpace.hh"
#include "Util/Exceptions/invalidArgumentException.hh"
#include "real.hh"
#include <iostream>
namespace Algorithm
{
  RealSolver::RealSolver(double value, std::shared_ptr<Interface::AbstractBanachSpace> domain, std::shared_ptr<Interface::AbstractBanachSpace> range)
    : AbstractLinearSolver(domain,range),
      value_(value)
  {}

  std::unique_ptr<Interface::AbstractFunctionSpaceElement> RealSolver::operator ()(const Interface::AbstractFunctionSpaceElement& y) const
  {
    if(  dynamic_cast<const Real*>(&y) == nullptr ) throw InvalidArgumentException("RealSolver::operator()(const FunctionSpaceElement&)");

    auto x = Algorithm::clone(y);
    *x *= 1./value_;
    return x;
  }

  RealSolver* RealSolver::cloneImpl() const
  {
    std::cout << "cloning real solver" << std::endl;
    return new RealSolver(value_,getSharedDomain(),getSharedRange());
  }
}
