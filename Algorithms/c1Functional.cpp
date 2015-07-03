#include "c1Functional.hh"

#include "functionSpaceElement.hh"
#include "Interface/Functional/abstractFunctional.hh"
#include "Interface/Functional/abstractC1Functional.hh"

namespace Algorithm
{
  C1Functional::C1Functional(std::unique_ptr<AbstractC1Functional>&& impl)
    : Functional(std::unique_ptr<AbstractFunctional>(impl.release()))
  {}

  double C1Functional::d1(const FunctionSpaceElement& x, const FunctionSpaceElement &dx) const
  {
    return dynamic_cast<const AbstractC1Functional&>(impl()).d1(x.impl(),dx.impl());
  }
}
