#include "differentiableFunctional.hh"

#include "functionSpaceElement.hh"
#include "Interface/abstractDifferentiableFunctional.hh"

namespace Algorithm
{
  DifferentiableFunctional::DifferentiableFunctional(std::shared_ptr<AbstractDifferentiableFunctional> impl)
    : Functional(impl),
      impl_(impl)
  {}

  double DifferentiableFunctional::d1(const FunctionSpaceElement &dx) const
  {
    return impl_->d1(dx.impl());
  }

  AbstractDifferentiableFunctional& DifferentiableFunctional::impl()
  {
    return *impl_;
  }

  const AbstractDifferentiableFunctional& DifferentiableFunctional::impl() const
  {
    return *impl_;
  }
}
