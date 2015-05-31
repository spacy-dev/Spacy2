#include "abstractC0Functional.hh"

namespace Algorithm
{
  AbstractC0Functional::AbstractC0Functional(const AbstractBanachSpace& domain)
    :domain_(domain)
  {}

  AbstractC0Functional::~AbstractC0Functional(){}

  double AbstractC0Functional::operator ()(const AbstractFunctionSpaceElement& x)
  {
    setArgument(x);
    return d0();
  }

  const AbstractBanachSpace& AbstractC0Functional::getDomain() const
  {
    return domain_;
  }
}
