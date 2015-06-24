#include "c2Functional.hh"

#include "c1Operator.hh"
#include "functionSpaceElement.hh"
#include "Interface/Functional/abstractC2Functional.hh"

namespace Algorithm
{
  C2Functional::C2Functional(std::shared_ptr<AbstractC2Functional> impl)
    : C1Functional(impl),
      impl_(impl)
  {}

  FunctionSpaceElement C2Functional::d2(const FunctionSpaceElement& x, const FunctionSpaceElement &dx) const
  {
    return FunctionSpaceElement( impl().getHessian( x.impl() )( dx.impl() ) );
    //return FunctionSpaceElement( impl().d2( x.impl() , dx.impl() ) );
  }

  double C2Functional::d2(const FunctionSpaceElement& x, const FunctionSpaceElement &dx, const FunctionSpaceElement& dy) const
  {
    auto dfdx = impl().getHessian( x.impl() ) ( dx.impl() );
    return (*dfdx) ( dy.impl() );
//    return impl().d2( x.impl() , dx.impl() ) ( dy.impl() );
  }

//  C1Operator C2Functional::getDerivative() const
//  {
//    return C1Operator( impl().getDerivative());
//  }


  AbstractC2Functional& C2Functional::impl()
  {
    return *impl_;
  }

  const AbstractC2Functional& C2Functional::impl() const
  {
    return *impl_;
  }
}
