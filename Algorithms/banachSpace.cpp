#include "banachSpace.hh"

#include "Interface/abstractBanachSpace.hh"
#include "functionSpaceElement.hh"

#include <utility>

namespace Algorithm
{
  BanachSpace::BanachSpace(std::shared_ptr<Interface::AbstractBanachSpace> implementation)
    : Mixin::SharedImpl<Interface::AbstractBanachSpace>(implementation), norm_(impl().getSharedNorm())
  {}

  void BanachSpace::setNorm(Norm&& norm)
  {
    norm_ = std::move(norm);
  }

  const Norm& BanachSpace::getNorm() const
  {
    return norm_;
  }

  FunctionSpaceElement BanachSpace::element() const
  {
    return FunctionSpaceElement(impl().element());
  }

  unsigned BanachSpace::index() const
  {
    return impl().index();
  }
}
