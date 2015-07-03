#include "c2Operator.hh"

#include "functionSpaceElement.hh"
#include "Interface/Operator/abstractC1Operator.hh"
#include "Interface/Operator/abstractC2Operator.hh"

namespace Algorithm
{
  C2Operator::C2Operator(std::unique_ptr<AbstractC2Operator>&& impl)
    : C1Operator(std::unique_ptr<AbstractC1Operator>(impl.release()))
  {}

  FunctionSpaceElement C2Operator::d2(const FunctionSpaceElement& x, const FunctionSpaceElement& dx, const FunctionSpaceElement& dy) const
  {
    return dynamic_cast<const AbstractC2Operator&>( impl() ).d2(x.impl(),dx.impl(),dy.impl());
  }
}
