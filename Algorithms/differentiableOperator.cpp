#include "differentiableOperator.hh"

#include "functionSpaceElement.hh"
#include "Interface/abstractDifferentiableOperator.hh"

namespace Algorithm
{
  DifferentiableOperator::DifferentiableOperator(std::shared_ptr<AbstractDifferentiableOperator> impl)
    : Operator(impl),
      impl_(impl)
  {}

  FunctionSpaceElement DifferentiableOperator::d1(const FunctionSpaceElement &dx) const
  {
    return FunctionSpaceElement( impl_->d1(dx.impl()) );
  }

  AbstractDifferentiableOperator& DifferentiableOperator::impl()
  {
    return *impl_;
  }

  const AbstractDifferentiableOperator& DifferentiableOperator::impl() const
  {
    return *impl_;
  }
}
