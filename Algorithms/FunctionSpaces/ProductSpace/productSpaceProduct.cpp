#include "productSpaceProduct.hh"

#include "productSpaceElement.hh"
#include "primalProductSpaceElement.hh"
#include "dualProductSpaceElement.hh"
#include "../../Util/incompatiblespaceexception.hh"
#include "../../Util/invalidargumentexception.hh"
#include "../../Interface/abstractHilbertSpace.hh"

namespace Algorithm
{
  double ProductSpaceProduct::operator()(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y) const
  {
    if( x.spaceIndex() != y.spaceIndex() ) throw IncompatibleSpaceException("ProductSpaceProduct",x.spaceIndex(),y.spaceIndex());
    if( !isAnyProductSpaceElement(x) && !isAnyProductSpaceElement(y) ) throw InvalidArgumentException("ProductSpaceProduct");

    auto result = 0.;

    if( !isDualProductSpaceElement(x) && !isDualProductSpaceElement(y) )
      for(auto i=0u; i<primalVariableSize(x); ++i)
        result += primalVariable(x,i) * primalVariable(y,i);

    if( !isPrimalProductSpaceElement(x) && !isPrimalProductSpaceElement(y) )
      for(auto i=0u; i<dualVariableSize(x); ++i)
        result += dualVariable(x,i) * dualVariable(y,i);

    return result;
  }
}
