#include "differentiableOperator.hh"

namespace Algorithm
{
  DifferentiableOperator::DifferentiableOperator(std::shared_ptr<AbstractDifferentiableOperator> impl)
    : Operator(impl),
      impl_(impl)
  {}

  FunctionSpaceElement DifferentiableOperator::d1(const FunctionSpaceElement &x, const FunctionSpaceElement &dx)
  {
    return impl_->d1(x,dx);
  }

  FunctionSpaceElement DifferentiableOperator::d1(const FunctionSpaceElement &dx) const
  {
    return impl_->d1(dx);
  }
}
