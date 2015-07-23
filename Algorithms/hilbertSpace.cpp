#include "hilbertSpace.hh"

#include "Interface/abstractBanachSpace.hh"

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
}
