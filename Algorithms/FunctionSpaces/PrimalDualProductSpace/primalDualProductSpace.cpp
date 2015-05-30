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
      primalSpaces_(std::move(primalSpaces)), dualSpaces_({})
  {}

  PrimalDualProductSpace::PrimalDualProductSpace(std::vector<std::unique_ptr<AbstractBanachSpace> >&& primalSpaces,
               std::vector<std::unique_ptr<AbstractBanachSpace> >&& dualSpaces)
    : AbstractHilbertSpace(std::make_shared<PrimalDualProductSpaceProduct>()),
      primalSpaces_(std::move(primalSpaces)),
      dualSpaces_( std::move(dualSpaces) )
  {}

  ProductSpace& PrimalDualProductSpace::getPrimalProductSpace()
  {
    return primalSpaces_;
  }

  const ProductSpace& PrimalDualProductSpace::getPrimalProductSpace() const
  {
    return primalSpaces_;
  }

  ProductSpace& PrimalDualProductSpace::getDualProductSpace()
  {
    return dualSpaces_;
  }

  const ProductSpace& PrimalDualProductSpace::getDualProductSpace() const
  {
    return dualSpaces_;
  }

  std::unique_ptr<AbstractFunctionSpaceElement> PrimalDualProductSpace::elementImpl() const
  {
    return std::make_unique<PrimalDualProductSpaceElement>(*this);
  }
}
