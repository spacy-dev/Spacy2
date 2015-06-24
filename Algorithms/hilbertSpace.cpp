#include "hilbertSpace.hh"

#include "Interface/abstractBanachSpace.hh"

namespace Algorithm
{
  HilbertSpace::HilbertSpace(std::shared_ptr<AbstractHilbertSpace> implementation)
    : BanachSpace( std::static_pointer_cast<AbstractBanachSpace>( implementation ) ),
      norm_(impl().getNorm()), sp_(impl().getScalarProduct())
  {}

//  HilbertSpace::HilbertSpace(const AbstractBanachSpace& implementation)
//    : HilbertSpace(implementation.clone())
//  {}

  void HilbertSpace::setScalarProduct(const ScalarProduct& sp)
  {
    sp_ = sp;
  }

  const ScalarProduct& HilbertSpace::getScalarProduct() const
  {
    return sp_;
  }

  AbstractHilbertSpace& HilbertSpace::impl()
  {
    return dynamic_cast<AbstractHilbertSpace&>(BanachSpace::impl());
  }

  AbstractHilbertSpace const& HilbertSpace::impl() const
  {
    return dynamic_cast<const AbstractHilbertSpace&>(BanachSpace::impl());
  }
}
