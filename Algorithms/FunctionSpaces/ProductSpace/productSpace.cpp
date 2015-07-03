#include "productSpace.hh"

#include "Util/Exceptions/invalidArgumentException.hh"

#include "hilbertSpaceNorm.hh"

#include "productSpaceElement.hh"
#include "productSpaceProduct.hh"

namespace Algorithm
{
  ProductSpace::ProductSpace(const std::vector<std::shared_ptr<AbstractBanachSpace> >& spaces)
    : AbstractHilbertSpace(std::make_shared<ProductSpaceProduct>()),
      spaces_(spaces)
  {}

  const std::vector<std::shared_ptr<AbstractBanachSpace> >& ProductSpace::getSpaces() const
  {
    return spaces_;
  }

  std::unique_ptr<AbstractFunctionSpaceElement> ProductSpace::elementImpl() const
  {
    return std::make_unique<ProductSpaceElement>(*this);
  }
}
