#include "abstractC0Functional.hh"

namespace Algorithm
{
  AbstractC0Functional::AbstractC0Functional(const AbstractBanachSpace& domain)
    :domain_(domain)
  {}

  AbstractC0Functional::~AbstractC0Functional(){}

  const AbstractBanachSpace& AbstractC0Functional::getDomain() const
  {
    return domain_;
  }
}
