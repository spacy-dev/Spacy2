#include "inducedScalarProduct.hh"

#include "abstractFunctionSpaceElement.hh"

#include "c2Functional.hh"
#include "functionSpaceElement.hh"

namespace Algorithm
{
  Interface::InducedScalarProduct::InducedScalarProduct(const AbstractC2Functional& F, const AbstractFunctionSpaceElement& x)
    : Mixin::UniqueImpl<Hessian>(clone(F.getHessian(x)))
  {}

  Interface::InducedScalarProduct::InducedScalarProduct(const C2Functional& F, const FunctionSpaceElement& x)
    : Mixin::UniqueImpl<Hessian>( clone( F.impl().getHessian(x.impl()) ) )
  {}

  double Interface::InducedScalarProduct::operator()(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y) const
  {
    auto dfdx = impl() ( x );
    return (*dfdx)( y );
  }
}
