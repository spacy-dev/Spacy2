#include "functional.hh"

#include "functionSpaceElement.hh"
#include "Interface/Functional/abstractFunctional.hh"

namespace Algorithm
{
  Functional::Functional(std::shared_ptr<AbstractFunctional> impl)
    : SharedImpl<AbstractFunctional>(impl)
  {}

  double Functional::operator()(const FunctionSpaceElement& x)
  {
    return impl()(x.impl());
  }
}
