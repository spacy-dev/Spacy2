#include "abstractFunctional.hh"

#include "Interface/abstractBanachSpace.hh"
#include "Util/invalidargumentexception.hh"

namespace Algorithm
{
  AbstractFunctional::AbstractFunctional(const AbstractBanachSpace& domain)
    : domain_(domain)
  {}

  AbstractFunctional::~AbstractFunctional(){}

  double AbstractFunctional::operator ()(const AbstractFunctionSpaceElement& x) const
  {
    if( getDomain().index() != x.getSpace().index() ) throw InvalidArgumentException("AbstractFunctional::operator() (incompatible space ids)");

    return d0(x);
  }

  const AbstractBanachSpace& AbstractFunctional::getDomain() const
  {
    return domain_;
  }
}
