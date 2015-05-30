#include "c2Functional.hh"

#include "Interface/abstractC2Functional.hh"

namespace Algorithm
{
  C2Functional::C2Functional(std::shared_ptr<AbstractC2Functional> impl)
    : C1Functional(impl),
      impl_(impl)
  {}

  double C2Functional::d2(const FunctionSpaceElement &dx, const FunctionSpaceElement& dy) const
  {
    return impl_->d2(dx.impl(),dy.impl());
  }


  AbstractC2Functional& C2Functional::impl()
  {
    return *impl_;
  }

  const AbstractC2Functional& C2Functional::impl() const
  {
    return *impl_;
  }
}
