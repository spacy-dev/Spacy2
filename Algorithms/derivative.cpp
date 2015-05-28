#include "derivative.hh"

namespace Algorithm
{
  Derivative::Derivative(const AbstractDifferentiableOperator &A, const AbstractFunctionSpaceElement& x)
    : A_(dynamic_cast<AbstractDifferentiableOperator*>(A.clone().release())),
      x_(x)
  {
    (*A_)(x_);
  }

  std::unique_ptr<AbstractOperator> Derivative::clone() const
  {
    return std::make_unique<Derivative>(*A_,x_);
  }

  std::unique_ptr<AbstractFunctionSpaceElement> Derivative::operator()(const AbstractFunctionSpaceElement& dx) const
  {
    return A_->d1(dx);
  }

  const FunctionSpace& Derivative::getDomain() const
  {
    return A_->getDomain();
  }

  const FunctionSpace& Derivative::getRange() const
  {
    return A_->getRange();
  }

  Operator derivative(const DifferentiableOperator& A, const FunctionSpaceElement& x)
  {
    return Operator(std::make_shared<Derivative>(A.impl(),x.impl()));
  }
}
