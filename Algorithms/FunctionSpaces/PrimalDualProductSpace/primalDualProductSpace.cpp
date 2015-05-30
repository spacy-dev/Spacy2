#include "primalDualProductSpace.hh"

#include "../../Util/invalidargumentexception.hh"

#include "../../hilbertSpaceNorm.hh"

#include "primalDualProductSpaceElement.hh"
#include "primalDualProductSpaceProduct.hh"

#include <utility>
#include <stdexcept>

namespace Algorithm
{
  PrimalDualProductSpace::PrimalDualProductSpace(std::vector<std::unique_ptr<AbstractBanachSpace> >&& primalSpaces)
    : AbstractHilbertSpace(std::make_shared<PrimalDualProductSpaceProduct>()),
      primalSpaces_(std::move(primalSpaces))
  {}

  PrimalDualProductSpace::PrimalDualProductSpace(std::vector<std::unique_ptr<AbstractBanachSpace> >&& primalSpaces,
               std::vector<std::unique_ptr<AbstractBanachSpace> >&& dualSpaces)
    : AbstractHilbertSpace(std::make_shared<PrimalDualProductSpaceProduct>()),
      primalSpaces_(std::move(primalSpaces)),
      dualSpaces_( std::make_unique<ProductSpace>( std::move(dualSpaces) ) )
  {}

  const ProductSpace& PrimalDualProductSpace::getPrimalProductSpace() const
  {
    return primalSpaces_;
  }

  const ProductSpace& PrimalDualProductSpace::getDualProductSpace() const
  {
    if( dualSpaces_ == nullptr ) throw std::logic_error("Trying to access nullpointer in PrimalDualPrimalDualProductSpace::getDualSpaces");
    return *dualSpaces_;
  }

  std::unique_ptr<AbstractFunctionSpaceElement> PrimalDualProductSpace::elementImpl() const
  {
    return std::make_unique<PrimalDualProductSpaceElement>(*this);
  }
}
