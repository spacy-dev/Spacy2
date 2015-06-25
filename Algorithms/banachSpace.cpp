#include "banachSpace.hh"

#include "Interface/abstractBanachSpace.hh"

#include <utility>

namespace Algorithm
{
  BanachSpace::BanachSpace(std::shared_ptr<AbstractBanachSpace> implementation)
    : impl_(std::move(implementation)), norm_(impl().getNorm())
  {}

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

  AbstractBanachSpace& BanachSpace::impl()
  {
    return *impl_;
  }

  AbstractBanachSpace const& BanachSpace::impl() const
  {
    return *impl_;
  }
}
