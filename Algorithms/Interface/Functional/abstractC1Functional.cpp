#include "abstractC1Functional.hh"

#include "Interface/abstractFunctionSpace.hh"

namespace Algorithm
{
  Interface::AbstractC1Functional::AbstractC1Functional(std::shared_ptr<AbstractFunctionSpace> domain)
    : AbstractFunctional(domain)
  {}
}
