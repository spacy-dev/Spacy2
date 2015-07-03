#include "functional.hh"

#include "functionSpaceElement.hh"

#include <utility>

namespace Algorithm
{
  Functional::Functional(std::unique_ptr<AbstractFunctional>&& impl)
    : Mixin::UniqueImpl<AbstractFunctional>(std::move(impl))
  {}

  double Functional::operator()(const FunctionSpaceElement& x)
  {
    return impl()(x.impl());
  }
}
