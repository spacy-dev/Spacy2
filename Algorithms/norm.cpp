#include "norm.hh"

Algorithm::Norm::Norm(std::shared_ptr<Interface::AbstractNorm> implementation)
  : Mixin::SharedImpl<Interface::AbstractNorm>(implementation)
{}

auto Algorithm::Norm::operator()(const FunctionSpaceElement& x) const-> decltype(std::declval<Interface::AbstractNorm>()(x.impl()))
{
  return impl()(x.impl());
}
