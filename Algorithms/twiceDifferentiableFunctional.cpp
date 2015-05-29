#include "twiceDifferentiableFunctional.hh"

#include "Interface/abstractTwiceDifferentiableFunctional.hh"

namespace Algorithm
{
  TwiceDifferentiableFunctional::TwiceDifferentiableFunctional(std::shared_ptr<AbstractTwiceDifferentiableFunctional> impl)
    : DifferentiableFunctional(impl),
      impl_(impl)
  {}

  double TwiceDifferentiableFunctional::d2(const FunctionSpaceElement &dx, const FunctionSpaceElement& dy) const
  {
    return impl_->d2(dx.impl(),dy.impl());
  }


  AbstractTwiceDifferentiableFunctional& TwiceDifferentiableFunctional::impl()
  {
    return *impl_;
  }

  const AbstractTwiceDifferentiableFunctional& TwiceDifferentiableFunctional::impl() const
  {
    return *impl_;
  }
}
