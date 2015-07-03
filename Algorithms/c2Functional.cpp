#include "c2Functional.hh"

#include "c1Operator.hh"
#include "functionSpaceElement.hh"
#include "Interface/Functional/abstractC1Functional.hh"
#include "Interface/Functional/abstractC2Functional.hh"

namespace Algorithm
{
  C2Functional::C2Functional(std::unique_ptr<AbstractC2Functional>&& impl)
    : C1Functional(std::unique_ptr<AbstractC1Functional>(impl.release()))
  {}

  FunctionSpaceElement C2Functional::d2(const FunctionSpaceElement& x, const FunctionSpaceElement &dx) const
  {
    return FunctionSpaceElement( dynamic_cast<const AbstractC2Functional&>( impl() ).getHessian( x.impl() )( dx.impl() ) );
  }

  double C2Functional::d2(const FunctionSpaceElement& x, const FunctionSpaceElement &dx, const FunctionSpaceElement& dy) const
  {
    auto dfdx = dynamic_cast<const AbstractC2Functional&>( impl() ).getHessian( x.impl() ) ( dx.impl() );
    return (*dfdx) ( dy.impl() );
  }
}
