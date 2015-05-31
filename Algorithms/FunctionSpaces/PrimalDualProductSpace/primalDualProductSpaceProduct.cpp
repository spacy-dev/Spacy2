#include "primalDualProductSpaceProduct.hh"

#include "primalDualProductSpaceElement.hh"
#include "Util/incompatiblespaceexception.hh"
#include "Util/invalidargumentexception.hh"
#include "Interface/abstractBanachSpace.hh"

namespace Algorithm
{
  double PrimalDualProductSpaceProduct::operator()(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y) const
  {
    if( x.spaceIndex() != y.spaceIndex() ) throw IncompatibleSpaceException("PrimalDualProductSpaceProduct",x.spaceIndex(),y.spaceIndex());
    if( !isPrimalDualProductSpaceElement(x) || !isPrimalDualProductSpaceElement(y) ) throw InvalidArgumentException("PrimalDualProductSpaceProduct");

    const auto& x_ = dynamic_cast<const PrimalDualProductSpaceElement&>(x);
    const auto& y_ = dynamic_cast<const PrimalDualProductSpaceElement&>(y);

    auto result = 0.;

    if( x_.isPrimalEnabled() && y_.isPrimalEnabled())
      for(auto i=0u; i<x_.primalVariableSize(); ++i)
        result += x_.primalVariable(i) * y_.primalVariable(i);

    if( x_.isDualEnabled() && y_.isDualEnabled())
      for(auto i=0u; i<x_.dualVariableSize(); ++i)
        result += x_.dualVariable(i) * y_.dualVariable(i);

    x_.reset();
    y_.reset();

    return result;
  }
}
