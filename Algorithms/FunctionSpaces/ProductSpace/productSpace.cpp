#include "productSpace.hh"

#include "Util/invalidargumentexception.hh"

#include "hilbertSpaceNorm.hh"

#include "productSpaceElement.hh"
#include "productSpaceProduct.hh"

#include <utility>

namespace Algorithm
{
  ProductSpace::ProductSpace(std::vector<std::unique_ptr<AbstractBanachSpace> >&& spaces)
    : AbstractHilbertSpace(std::make_shared<ProductSpaceProduct>()),
      spaces_(std::move(spaces))
  {}

  const std::vector<std::unique_ptr<AbstractBanachSpace> >& ProductSpace::getSpaces() const
  {
    return spaces_;
  }

  std::unique_ptr<AbstractFunctionSpaceElement> ProductSpace::elementImpl() const
  {
    return std::make_unique<ProductSpaceElement>(*this);
  }
}
