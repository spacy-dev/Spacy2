#include "abstractFunctional.hh"

namespace Algorithm
{
  AbstractFunctional::AbstractFunctional(const AbstractBanachSpace& domain)
    : domain_(domain)
  {}

  AbstractFunctional::~AbstractFunctional(){}

  const AbstractBanachSpace& AbstractFunctional::getDomain() const
  {
    return domain_;
  }
}
