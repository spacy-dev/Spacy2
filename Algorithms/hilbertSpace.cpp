#include "hilbertSpace.hh"

#include "Interface/abstractBanachSpace.hh"
#include "Util/Exceptions/invalidArgumentException.hh"
#include "Util/castTo.hh"

namespace Algorithm
{
  HilbertSpace::HilbertSpace(std::shared_ptr<Interface::AbstractHilbertSpace> implementation)
    : BanachSpace( std::static_pointer_cast<Interface::AbstractBanachSpace>( implementation ) )
  {
    impl().setDualSpace( sharedImpl() );
  }

  void HilbertSpace::setScalarProduct(const ScalarProduct& sp)
  {
    castTo<Interface::AbstractHilbertSpace>( impl() ).setScalarProduct( sp.sharedImpl() );
  }

  ScalarProduct HilbertSpace::scalarProduct() const
  {
    return ScalarProduct( dynamic_cast<const Interface::AbstractHilbertSpace&>( impl() ).scalarProduct() );
  }


}
