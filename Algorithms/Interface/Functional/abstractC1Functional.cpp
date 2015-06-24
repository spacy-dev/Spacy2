#include "abstractC1Functional.hh"

#include "Interface/abstractFunctionSpaceElement.hh"

namespace Algorithm
{
  AbstractC1Functional::AbstractC1Functional(const AbstractBanachSpace &domain)
    : AbstractFunctional(domain)
  {}

//  LinearizedOperator AbstractC1Functional::getLinearization(const AbstractFunctionSpaceElement& x) const
//  {
//    return makeLinearization(x);
//  }

//  LinearizedOperator AbstractC1Functional::makeLinearization(const AbstractFunctionSpaceElement& x) const
//  {
//    return LinearizedOperator(*this);
//  }

//  double AbstractC1Functional::d0() const
//  {
//    return (*this)(getArgument());
//  }

//  void AbstractC1Functional::setArgument(const AbstractFunctionSpaceElement &x)
//  {
//    x_ = Algorithm::clone(x);
//  }

//  const AbstractFunctionSpaceElement& AbstractC1Functional::getArgument() const
//  {
//    return *x_;
//  }

}
