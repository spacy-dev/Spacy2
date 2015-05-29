#include "dualPairing.hh"

Algorithm::DualPairing::DualPairing(std::shared_ptr<AbstractDualPairing> implementation)
  : impl_(std::move(implementation))
{}

auto Algorithm::DualPairing::operator()(const FunctionSpaceElement& x, const FunctionSpaceElement& y) const -> decltype(std::declval<AbstractDualPairing>()(x.impl(),y.impl()))
{
  return (*impl_)(x.impl(),y.impl());
}
