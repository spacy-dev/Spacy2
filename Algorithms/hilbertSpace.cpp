#include "hilbertSpace.hh"

#include "Interface/abstractBanachSpace.hh"
#include "hilbertSpaceNorm.hh"

namespace Algorithm
{
  HilbertSpace::HilbertSpace(std::shared_ptr<AbstractHilbertSpace> implementation)
    : BanachSpace( std::static_pointer_cast<AbstractBanachSpace>( implementation ) ),
      sp_(implementation->getScalarProduct())
  {}

  void HilbertSpace::setScalarProduct(const ScalarProduct& sp)
  {
    dynamic_cast<AbstractHilbertSpace&>( impl() ).setScalarProduct( sp.sharedImpl() );
  }

  ScalarProduct HilbertSpace::getScalarProduct() const
  {
    return ScalarProduct( dynamic_cast<const AbstractHilbertSpace&>( impl() ).getScalarProduct() );
  }
}
