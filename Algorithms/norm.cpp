#include "norm.hh"

Algorithm::Norm::Norm(std::shared_ptr<AbstractNorm> implementation)
  : SharedImpl<AbstractNorm>(implementation)
{}

auto Algorithm::Norm::operator()(const FunctionSpaceElement& x) const-> decltype(std::declval<AbstractNorm>()(x.impl()))
{
  return impl()(x.impl());
}
