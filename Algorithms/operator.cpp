#include "operator.hh"

#include "functionSpaceElement.hh"

namespace Algorithm
{
  Operator::Operator(std::shared_ptr<AbstractOperator> impl)
    : Mixin::SharedImpl<AbstractOperator>(impl)
  {}

  FunctionSpaceElement Operator::operator()(const FunctionSpaceElement& x) const
  {
    return impl()(x.impl());
  }
}
