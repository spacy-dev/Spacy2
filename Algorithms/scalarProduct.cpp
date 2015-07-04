#include "scalarProduct.hh"

Algorithm::ScalarProduct::ScalarProduct(std::shared_ptr<Interface::AbstractScalarProduct> implementation)
  : Mixin::SharedImpl<Interface::AbstractScalarProduct>(implementation)
{}

auto Algorithm::ScalarProduct::operator()(const FunctionSpaceElement& x, const FunctionSpaceElement& y) const -> decltype(std::declval<Interface::AbstractScalarProduct>()(x.impl(),y.impl()))
{
  return impl()(x.impl(),y.impl());
}
