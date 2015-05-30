#include "c1Functional.hh"

#include "functionSpaceElement.hh"
#include "Interface/abstractC1Functional.hh"

namespace Algorithm
{
  C1Functional::C1Functional(std::shared_ptr<AbstractC1Functional> impl)
    : C0Functional(impl),
      impl_(impl)
  {}

  double C1Functional::d1(const FunctionSpaceElement &dx) const
  {
    return impl_->d1(dx.impl());
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
