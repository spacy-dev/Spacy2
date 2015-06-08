#include "abstractC1Functional.hh"

#include "Interface/abstractFunctionSpaceElement.hh"

namespace Algorithm
{
  AbstractC1Functional::AbstractC1Functional(const AbstractBanachSpace &domain)
    : AbstractFunctional(domain)
  {}

  double AbstractC1Functional::d0() const
  {
    return (*this)(getArgument());
  }

  void AbstractC1Functional::setArgument(const AbstractFunctionSpaceElement &x)
  {
    x_ = Algorithm::clone(x);
  }

  const AbstractFunctionSpaceElement& AbstractC1Functional::getArgument() const
  {
    return *x_;
  }

}
