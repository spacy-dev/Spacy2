#include "scalarProduct.hh"

Algorithm::ScalarProduct::ScalarProduct(std::shared_ptr<AbstractDualPairing> implementation)
  : impl_(std::move(implementation))
{}

auto Algorithm::ScalarProduct::operator()(const FunctionSpaceElement& x, const FunctionSpaceElement& y) const -> decltype(std::declval<AbstractDualPairing>()(x.impl(),y.impl()))
{
  return impl()(x.impl(),y.impl());
}

Algorithm::AbstractDualPairing const& Algorithm::ScalarProduct::impl() const
{
  return *impl_;
}

