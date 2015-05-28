#include "scalarProduct.hh"

Algorithm::ScalarProduct::ScalarProduct(std::shared_ptr<AbstractScalarProduct> implementation)
  : impl(std::move(implementation))
{}

auto Algorithm::ScalarProduct::operator()(const FunctionSpaceElement& x, const FunctionSpaceElement& y) const -> decltype(std::declval<AbstractScalarProduct>()(x.impl(),y.impl()))
{
  return (*impl)(x.impl(),y.impl());
}
