#include "operator.hh"

#include "functionSpaceElement.hh"
#include <utility>
namespace Algorithm
{
  Operator::Operator(std::unique_ptr<AbstractOperator>&& impl)
    : Mixin::UniqueImpl<AbstractOperator>(std::move(impl))
  {}

  FunctionSpaceElement Operator::operator()(const FunctionSpaceElement& x) const
  {
    return impl()(x.impl());
  }
}
