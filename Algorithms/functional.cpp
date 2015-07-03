#include "functional.hh"

#include "functionSpaceElement.hh"
#include "Interface/Functional/abstractFunctional.hh"

namespace Algorithm
{
  Functional::Functional(std::shared_ptr<AbstractFunctional> impl)
    : Mixin::SharedImpl<AbstractFunctional>(impl)
  {}

  double Functional::operator()(const FunctionSpaceElement& x)
  {
    return impl()(x.impl());
  }
}
