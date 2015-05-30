#include "derivative.hh"

#include "functionSpaceElement.hh"

#include "Interface/abstractC1Operator.hh"

namespace Algorithm
{
  Derivative::Derivative(const AbstractC1Operator &A, const AbstractFunctionSpaceElement& x)
    : A_(dynamic_cast<AbstractC1Operator*>(A.clone().release())),
      x_(x)
  {
    (*A_)(x_);
  }

  std::unique_ptr<AbstractC0Operator> Derivative::clone() const
  {
    return std::make_unique<Derivative>(*A_,x_);
  }

  std::unique_ptr<AbstractFunctionSpaceElement> Derivative::operator()(const AbstractFunctionSpaceElement& dx) const
  {
    return A_->d1(dx);
  }

  const AbstractBanachSpace& Derivative::getDomain() const
  {
    return A_->getDomain();
  }

  const AbstractBanachSpace& Derivative::getRange() const
  {
    return A_->getRange();
  }

  C0Operator derivative(const C1Operator& A, const FunctionSpaceElement& x)
  {
    return C0Operator(std::make_shared<Derivative>(A.impl(),x.impl()) );
  }
}
