#include "abstractFunctionSpaceElement.hh"

#include "abstractBanachSpace.hh"

#include "Util/invalidargumentexception.hh"

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
    auto z = clone(y);
    *z *= a;
    return *this +=*z;
  }

  double AbstractFunctionSpaceElement::operator()(const AbstractFunctionSpaceElement& y) const
  {
    if( !getSpace().isDualWRT( y.getSpace() ) ) throw InvalidArgumentException("AbstractFunctionSpaceElement::operator()");
    return applyAsDualTo(y);
  }

  std::ostream& operator<<(std::ostream& os, const AbstractFunctionSpaceElement& element)
  {
    element.print(os);
    return os;
  }
}
