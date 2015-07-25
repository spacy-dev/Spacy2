#include "abstractC1Functional.hh"

namespace Algorithm
{
  Interface::AbstractC1Functional::AbstractC1Functional(std::shared_ptr<AbstractBanachSpace> domain)
    : AbstractFunctional(domain)
  {}
}
