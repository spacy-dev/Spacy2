#include "c1Functional.hh"

#include "functionSpaceElement.hh"
#include "Interface/Functional/abstractC1Functional.hh"

namespace Algorithm
{
  C1Functional::C1Functional(std::shared_ptr<AbstractC1Functional> impl)
    : Functional(impl),
      impl_(impl)
  {}

  void C1Functional::setArgument(const FunctionSpaceElement &x)
  {
    impl().setArgument(x.impl());
  }

  double C1Functional::operator ()() const
  {
    return impl().d0();
  }

  double C1Functional::d1(const FunctionSpaceElement &dx) const
  {
    return impl().d1(dx.impl());
  }

  FunctionSpaceElement C1Functional::d1() const
  {
    return impl().d1();
  }

  AbstractC1Functional& C1Functional::impl()
  {
    return *impl_;
  }

  const AbstractC1Functional& C1Functional::impl() const
  {
    return *impl_;
  }
}
