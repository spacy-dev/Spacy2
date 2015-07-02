#include "banachSpace.hh"

#include "Interface/abstractBanachSpace.hh"
#include "functionSpaceElement.hh"

namespace Algorithm
{
  BanachSpace::BanachSpace(std::shared_ptr<AbstractBanachSpace> implementation)
    : SharedImpl<AbstractBanachSpace>(implementation), norm_(impl().getSharedNorm())
  {}

  void BanachSpace::setNorm(const Norm &norm)
  {
    norm_ = norm;
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
