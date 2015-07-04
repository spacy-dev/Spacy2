#include "operator.hh"

#include "functionSpaceElement.hh"
#include <utility>

namespace Algorithm
{
  Operator::Operator(std::unique_ptr<Interface::AbstractOperator>&& impl)
    : Mixin::UniqueImpl<Interface::AbstractOperator>(std::move(impl))
  {}

  FunctionSpaceElement Operator::operator()(const FunctionSpaceElement& x) const
  {
    return impl()(x.impl());
  }
}
