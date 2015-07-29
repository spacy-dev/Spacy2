#include "abstractFunctionSpaceElement.hh"

#include "abstractBanachSpace.hh"
#include "abstractNorm.hh"

#include "Util/Exceptions/invalidArgumentException.hh"
#include <iostream>
namespace Algorithm
{
  namespace Interface
  {
    AbstractFunctionSpaceElement::AbstractFunctionSpaceElement(const AbstractBanachSpace& space)
      : space_(space)
    {}

    unsigned AbstractFunctionSpaceElement::spaceIndex() const
    {
      return space_.index();
    }

    const AbstractBanachSpace& AbstractFunctionSpaceElement::space() const
    {
      return space_;
    }

    bool AbstractFunctionSpaceElement::equals(const AbstractFunctionSpaceElement& y) const
    {
      auto y_ = clone(y);
      *y_ -= *this;
      const auto& norm_ = space().norm();
      return norm_(*y_) < eps();
    }

    bool AbstractFunctionSpaceElement::isAdmissible() const
    {
      return true;
    }


    AbstractFunctionSpaceElement& AbstractFunctionSpaceElement::axpy(double a, const AbstractFunctionSpaceElement& y)
    {
      auto z = clone(y);
      *z *= a;
      return *this +=*z;
    }

    double AbstractFunctionSpaceElement::operator()(const AbstractFunctionSpaceElement& y) const
    {
      if( !space().isDualWRT( y.space() ) ) throw InvalidArgumentException("AbstractFunctionSpaceElement::operator()");
      return applyAsDualTo(y);
    }

    std::ostream& operator<<(std::ostream& os, const AbstractFunctionSpaceElement& element)
    {
      element.print(os);
      return os;
    }
  }
}
