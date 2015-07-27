#include "hilbertSpace.hh"

#include "Interface/abstractBanachSpace.hh"
#include "Util/Exceptions/invalidArgumentException.hh"

namespace Algorithm
{
  HilbertSpace::HilbertSpace(std::shared_ptr<Interface::AbstractHilbertSpace> implementation)
    : BanachSpace( std::static_pointer_cast<Interface::AbstractBanachSpace>( implementation ) )
  {}

  void HilbertSpace::setScalarProduct(const ScalarProduct& sp)
  {
    dynamic_cast<Interface::AbstractHilbertSpace&>( impl() ).setScalarProduct( sp.sharedImpl() );
  }

  ScalarProduct HilbertSpace::getScalarProduct() const
  {
    return ScalarProduct( dynamic_cast<const Interface::AbstractHilbertSpace&>( impl() ).getScalarProduct() );
  }

  HilbertSpace& castToHilbertSpace(BanachSpace& space)
  {
    if( dynamic_cast<HilbertSpace*>(&space) == nullptr ) throw InvalidArgumentException("castToHilbertSpace(BanachSpace&)");
    return dynamic_cast<HilbertSpace&>(space);
  }

  const HilbertSpace& castToHilbertSpace(const BanachSpace& space)
  {
    if( dynamic_cast<const HilbertSpace*>(&space) == nullptr ) throw InvalidArgumentException("castToHilbertSpace(BanachSpace&)");
    return dynamic_cast<const HilbertSpace&>(space);
  }


}
