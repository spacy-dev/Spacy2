#include "abstractFunctionSpaceElement.hh"

#include "abstractBanachSpace.hh"

namespace Algorithm
{
  AbstractFunctionSpaceElement::AbstractFunctionSpaceElement(const AbstractBanachSpace& space)
    : space_(space)
  {}

  unsigned AbstractFunctionSpaceElement::spaceIndex() const
  {
    return space_.index();
  }

  const AbstractBanachSpace& AbstractFunctionSpaceElement::getSpace() const
  {
    return space_;
  }

  std::ostream& operator<<(std::ostream& os, const AbstractFunctionSpaceElement& element)
  {
    element.print(os);
    return os;
  }
}
