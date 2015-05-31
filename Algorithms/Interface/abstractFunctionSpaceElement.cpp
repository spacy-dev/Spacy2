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

  AbstractFunctionSpaceElement& AbstractFunctionSpaceElement::axpy(double a, const AbstractFunctionSpaceElement& y)
  {
    auto z = y.clone();
    *z *= a;
    return *this +=*z;
  }

  std::ostream& operator<<(std::ostream& os, const AbstractFunctionSpaceElement& element)
  {
    element.print(os);
    return os;
  }
}
