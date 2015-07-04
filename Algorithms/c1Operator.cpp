#include "c1Operator.hh"

#include "functionSpaceElement.hh"
#include "Interface/Operator/abstractC1Operator.hh"
#include "Interface/Operator/linearizedOperator.hh"

namespace Algorithm
{
  C1Operator::C1Operator(std::unique_ptr<Interface::AbstractC1Operator>&& impl)
    : Operator(std::unique_ptr<Interface::AbstractOperator>(impl.release()))
  {}

  FunctionSpaceElement C1Operator::d1(const FunctionSpaceElement& x, const FunctionSpaceElement& dx) const
  {
    return FunctionSpaceElement( dynamic_cast<const Interface::AbstractC1Operator&>( impl() ).getLinearization(x.impl())( dx.impl() ) );
  }

  LinearOperator C1Operator::getLinearization(const FunctionSpaceElement& x) const
  {
    return LinearOperator( dynamic_cast<const Interface::AbstractC1Operator&>( impl() ).getLinearization(x.impl()) );
  }
}
