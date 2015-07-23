#include "c2Functional.hh"

#include "c1Operator.hh"
#include "functionSpaceElement.hh"
#include "Interface/Functional/abstractC1Functional.hh"
#include "Interface/Functional/abstractC2Functional.hh"
#include "Interface/Functional/hessian.hh"

namespace Algorithm
{
  C2Functional::C2Functional(std::unique_ptr<Interface::AbstractC2Functional>&& impl)
    : C1Functional(std::unique_ptr<Interface::AbstractC1Functional>(impl.release()))
  {}

  FunctionSpaceElement C2Functional::d2(const FunctionSpaceElement& x, const FunctionSpaceElement &dx) const
  {
    return FunctionSpaceElement( impl().getHessian( x.impl() )( dx.impl() ) );
  }

  double C2Functional::d2(const FunctionSpaceElement& x, const FunctionSpaceElement &dx, const FunctionSpaceElement& dy) const
  {
    return impl().d2( x.impl() , dx.impl() , dy.impl() );
//    auto dfdx = impl().getHessian( x.impl() ) ( dx.impl() );
//    return (*dfdx) ( dy.impl() );
  }

  LinearOperator C2Functional::hessian(const FunctionSpaceElement& x) const
  {
    return LinearOperator( impl().getHessian( x.impl() ) );
  }


  Interface::AbstractC2Functional& C2Functional::impl()
  {
    return dynamic_cast<Interface::AbstractC2Functional&>( C1Functional::impl() );
  }

  const Interface::AbstractC2Functional& C2Functional::impl() const
  {
    return dynamic_cast<const Interface::AbstractC2Functional&>( C1Functional::impl() );
  }
}
