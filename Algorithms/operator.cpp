#include "operator.hh"

#include "functionSpaceElement.hh"

namespace Algorithm
{
  Operator::Operator(std::shared_ptr<AbstractOperator> impl)
    : impl_(impl)
  {}

  FunctionSpaceElement Operator::operator()(const FunctionSpaceElement& x) const
  {
    return impl()(x.impl());
  }

  AbstractOperator& Operator::impl()
  {
    return *impl_;
  }

  const AbstractOperator& Operator::impl() const
  {
    return *impl_;
  }
}
