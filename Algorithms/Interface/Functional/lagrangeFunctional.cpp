#include "lagrangeFunctional.hh"

#include "Interface/abstractBanachSpace.hh"

#include <utility>

namespace Algorithm
{
  Interface::AbstractLagrangeFunctional::AbstractLagrangeFunctional(std::shared_ptr<AbstractBanachSpace> domain)
    : AbstractC2Functional(domain)
  {}
}
