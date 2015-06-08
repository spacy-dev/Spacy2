#include "functional.hh"

#include "functionSpaceElement.hh"
#include "Interface/Functional/abstractFunctional.hh"

namespace Algorithm
{
  Functional::Functional(std::shared_ptr<AbstractFunctional> impl)
    : impl_(impl)
  {}

  double Functional::operator()(const FunctionSpaceElement& x)
  {
    return impl()(x.impl());
  }

  AbstractFunctional& Functional::impl()
  {
    return *impl_;
  }

  const AbstractFunctional& Functional::impl() const
  {
    return *impl_;
  }
}
