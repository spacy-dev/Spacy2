#include "abstractHilbertSpace.hh"

#include "abstractFunctionSpaceElement.hh"
#include "abstractDualPairing.hh"
#include "abstractNorm.hh"
#include "abstractDualPairing.hh"
#include "../hilbertSpaceNorm.hh"

namespace Algorithm
{
  AbstractHilbertSpace::AbstractHilbertSpace(std::shared_ptr<AbstractDualPairing> sp)
    : AbstractBanachSpace(this,std::make_shared<HilbertSpaceNorm>(sp))
  {
    this->dp_ = sp;
  }

  void AbstractHilbertSpace::setScalarProduct(std::shared_ptr<AbstractDualPairing> sp)
  {
    setDualPairing(sp);
    setNorm(std::make_shared<HilbertSpaceNorm>(sp));
  }

  std::shared_ptr<AbstractDualPairing> AbstractHilbertSpace::getScalarProduct() const
  {
    return getDualPairing();
  }

  bool isHilbertSpace(const AbstractBanachSpace& space)
  {
    return dynamic_cast<const AbstractHilbertSpace*>(&space) != nullptr;
  }
}
