#include "derivative.hh"

namespace Algorithm
{
  Derivative::Derivative(const DifferentiableOperator& A)
    : A_(A), dx_( A_.getDomain().element() )
  {}

  void Derivative::setArgument(const FunctionSpaceElement& dx)
  {
    dx_ = dx;
  }

  FunctionSpaceElement Derivative::operator()() const
  {
    return A_.d1(dx_);
  }

  const FunctionSpace& Derivative::getDomain() const
  {
    return A_.getDomain();
  }

  const FunctionSpace& Derivative::getRange() const
  {
    return A_.getRange();
  }

  Operator derivative(const DifferentiableOperator& A)
  {
    return Operator(std::make_shared<Derivative>(A));
  }
}
