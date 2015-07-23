#include "abstractC2Functional.hh"

#include "Interface/abstractBanachSpace.hh"
#include "Interface/abstractLinearSolver.hh"
#include "hessian.hh"

namespace Algorithm
{
  Interface::AbstractC2Functional::AbstractC2Functional(std::shared_ptr<AbstractBanachSpace> domain)
    : AbstractC1Functional(domain)
  {}

  Interface::Hessian Interface::AbstractC2Functional::getHessian(const AbstractFunctionSpaceElement& x) const
  {
    return makeHessian(x);
  }

  Interface::Hessian Interface::AbstractC2Functional::makeHessian(const AbstractFunctionSpaceElement& x) const
  {
    return Interface::Hessian(clone(this),x,solver_);
  }

  double Interface::AbstractC2Functional::d2(const AbstractFunctionSpaceElement &x, const AbstractFunctionSpaceElement &dx, const AbstractFunctionSpaceElement& dy) const
  {
    return dy(*d2(x,dx));
  }
}
