#include "abstractBanachSpace.hh"

namespace Algorithm
{
  std::shared_ptr<AbstractNorm> AbstractBanachSpace::getNorm() const
  {
    return getNormImpl();
  }

  std::unique_ptr<AbstractFunctionSpaceElement> AbstractBanachSpace::element() const
  {
    return elementImpl();
  }

  unsigned AbstractBanachSpace::index() const
  {
    return index_;
  }
}
