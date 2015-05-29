#include "functionSpaceElement.hh"

#include "Interface/abstractBanachSpace.hh"
#include "Interface/abstractFunctionSpaceElement.hh"
#include "functionSpace.hh"

#include <utility>

namespace Algorithm
{
  FunctionSpaceElement::FunctionSpaceElement(std::unique_ptr<AbstractFunctionSpaceElement>&& implementation)
    : impl_(std::move(implementation))
  {}

  FunctionSpaceElement::FunctionSpaceElement(const FunctionSpaceElement& y)
    : impl_(y.impl().clone())
  {}

  FunctionSpaceElement& FunctionSpaceElement::operator=(const FunctionSpaceElement& y)
  {
    y.impl().copyTo(*impl_);
    return *this;
  }

  void FunctionSpaceElement::print(std::ostream& os) const
  {
    return impl_->print(os);
  }

  AbstractFunctionSpaceElement& FunctionSpaceElement::impl()
  {
    return *impl_;
  }

  const AbstractFunctionSpaceElement& FunctionSpaceElement::impl() const
  {
    return *impl_;
  }

  unsigned FunctionSpaceElement::spaceIndex() const
  {
    return impl_->getSpace().index();
  }

  bool FunctionSpaceElement::operator==(const FunctionSpaceElement& y) const
  {
    return (this == &y);
  }


  FunctionSpaceElement& FunctionSpaceElement::operator+=(const FunctionSpaceElement& y)
  {
    *impl_ += y.impl();
    return *this;
  }

  FunctionSpaceElement& FunctionSpaceElement::operator-=(const FunctionSpaceElement& y)
  {
    *impl_ -= y.impl();
    return *this;
  }

  FunctionSpaceElement FunctionSpaceElement::operator-() const
  {
    return FunctionSpaceElement(-(*impl_));
  }

  double& FunctionSpaceElement::coefficient(unsigned i)
  {
    return impl_->coefficient(i);
  }

  const double& FunctionSpaceElement::coefficient(unsigned i) const
  {
    return impl_->coefficient(i);
  }

  unsigned FunctionSpaceElement::size() const
  {
    return impl_->size();
  }

  // free functions
  auto operator*(const FunctionSpaceElement& x, const FunctionSpaceElement& y) -> decltype(x.impl()*y.impl())
  {
    return x.impl() * y.impl();
  }

  std::ostream& operator<<(std::ostream& os, const FunctionSpaceElement& x)
  {
    return os << x.impl();
  }
}
