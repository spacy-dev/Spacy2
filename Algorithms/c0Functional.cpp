#include "c0Functional.hh"

#include "functionSpaceElement.hh"
#include "Interface/abstractC0Functional.hh"

namespace Algorithm
{
  C0Functional::C0Functional(std::shared_ptr<AbstractC0Functional> impl)
    : impl_(impl)
  {}

  void C0Functional::setArgument(const FunctionSpaceElement &x)
  {
    impl().setArgument(x.impl());
  }

  double C0Functional::operator()() const
  {
    return impl().d0();
  }

  double C0Functional::operator()(const FunctionSpaceElement& x)
  {
    return impl()(x.impl());
  }

  AbstractC0Functional& C0Functional::impl()
  {
    return *impl_;
  }

  const AbstractC0Functional& C0Functional::impl() const
  {
    return *impl_;
  }
}
