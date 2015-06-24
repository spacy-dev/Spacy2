#include "scalarProduct.hh"

Algorithm::ScalarProduct::ScalarProduct(std::shared_ptr<AbstractScalarProduct> implementation)
  : impl_(std::move(implementation))
{}

auto Algorithm::ScalarProduct::operator()(const FunctionSpaceElement& x, const FunctionSpaceElement& y) const -> decltype(std::declval<AbstractScalarProduct>()(x.impl(),y.impl()))
{
  return impl()(x.impl(),y.impl());
}

Algorithm::AbstractScalarProduct const& Algorithm::ScalarProduct::impl() const
{
  return *impl_;
}

