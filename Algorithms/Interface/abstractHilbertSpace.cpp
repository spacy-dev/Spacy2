#include "abstractHilbertSpace.hh"

#include "abstractFunctionSpaceElement.hh"
#include "abstractDualPairing.hh"
#include "../hilbertSpaceNorm.hh"

namespace Algorithm
{
  AbstractHilbertSpace::AbstractHilbertSpace()
    : norm_(nullptr)
  {
    dualSpace_ = this;
  }

  void AbstractHilbertSpace::setScalarProduct(std::shared_ptr<AbstractDualPairing> sp)
  {
    setScalarProductImpl(sp);
    setNorm(std::make_shared<HilbertSpaceNorm>(getScalarProduct()));
  }

  void AbstractHilbertSpace::setNormImpl(std::shared_ptr<AbstractNorm> norm)
  {
    norm_ = norm;
  }


  std::shared_ptr<AbstractDualPairing> AbstractHilbertSpace::getScalarProduct() const
  {
    return getScalarProductImpl();
  }

  std::shared_ptr<AbstractNorm> AbstractHilbertSpace::getNormImpl() const
  {
    return norm_;
  }

  void AbstractHilbertSpace::setDualPairingImpl(std::shared_ptr<AbstractDualPairing> dp)
  {
    setScalarProductImpl(dp);
  }

  std::shared_ptr<AbstractDualPairing> AbstractHilbertSpace::getDualPairingImpl() const
  {
    return getScalarProductImpl();
  }


  bool isHilbertSpace(const AbstractBanachSpace& space)
  {
    return dynamic_cast<const AbstractHilbertSpace*>(&space) != nullptr;
  }
}
