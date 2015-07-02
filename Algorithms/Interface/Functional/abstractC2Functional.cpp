#include "abstractC2Functional.hh"

#include "Interface/abstractBanachSpace.hh"

#include <utility>

namespace Algorithm
{
  AbstractC2Functional::AbstractC2Functional(std::shared_ptr<AbstractBanachSpace> domain)
    : AbstractC1Functional(domain)
  {}

  Hessian AbstractC2Functional::getHessian(const AbstractFunctionSpaceElement& x) const
  {
    return makeHessian(x);
  }

  Hessian AbstractC2Functional::makeHessian(const AbstractFunctionSpaceElement& x) const
  {
    return Hessian(*this,x);
  }

//  double AbstractC2Functional::d1(const AbstractFunctionSpaceElement &dx) const
//  {
//    return (*(derivative_->d0())) ( dx );
//  }

//  std::shared_ptr<AbstractC1Operator> AbstractC2Functional::getDerivative() const
//  {
//    return derivative_;
//  }

//  void AbstractC2Functional::setDerivative(std::shared_ptr<AbstractC1Operator> derivative)
//  {
//    derivative_ = derivative;
//  }
}
