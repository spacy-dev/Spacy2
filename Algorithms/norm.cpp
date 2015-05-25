#include "norm.hh"

Algorithm::Norm::Norm(std::shared_ptr<AbstractNorm> implementation)
  : impl_(std::move(implementation))
{}

auto Algorithm::Norm::operator()(const FunctionSpaceElement& x) const-> decltype(std::declval<AbstractNorm>()(x.impl()))
{
  return (*impl_)(x.impl());
}
