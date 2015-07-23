#include "functional.hh"

#include "functionSpaceElement.hh"

#include <utility>

namespace Algorithm
{
  Functional::Functional(std::unique_ptr<Interface::AbstractFunctional>&& impl)
    : Mixin::UniqueImpl<Interface::AbstractFunctional>(std::move(impl))
  {}

  double Functional::operator()(const FunctionSpaceElement& x) const
  {
    return impl()(x.impl());
  }
}
