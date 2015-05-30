#include "c1Operator.hh"

#include "functionSpaceElement.hh"
#include "Interface/abstractC1Operator.hh"

namespace Algorithm
{
  C1Operator::C1Operator(std::shared_ptr<AbstractC1Operator> impl)
    : C0Operator(impl),
      impl_(impl)
  {}

  FunctionSpaceElement C1Operator::d1(const FunctionSpaceElement &dx) const
  {
    return FunctionSpaceElement( impl_->d1(dx.impl()) );
  }

  AbstractC1Operator& C1Operator::impl()
  {
    return *impl_;
  }

  const AbstractC1Operator& C1Operator::impl() const
  {
    return *impl_;
  }
}
