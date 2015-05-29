#include "twiceDifferentiableOperator.hh"

namespace Algorithm
{
  TwiceDifferentiableOperator::TwiceDifferentiableOperator(std::shared_ptr<AbstractTwiceDifferentiableOperator> impl)
    : DifferentiableOperator(impl),
      impl_(impl)
  {}

  FunctionSpaceElement TwiceDifferentiableOperator::d2(const FunctionSpaceElement &dx, const FunctionSpaceElement& dy) const
  {
    return FunctionSpaceElement( impl_->d2(dx.impl(),dy.impl()) );
  }


  AbstractTwiceDifferentiableOperator& TwiceDifferentiableOperator::impl()
  {
    return *impl_;
  }

  const AbstractTwiceDifferentiableOperator& TwiceDifferentiableOperator::impl() const
  {
    return *impl_;
  }
}
