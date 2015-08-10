#include "productSpaceProduct.hh"

#include "productSpaceElement.hh"
#include "Util/Exceptions/incompatibleSpaceException.hh"
#include "Util/Exceptions/invalidArgumentException.hh"
#include "Interface/abstractFunctionSpace.hh"
#include "Interface/abstractFunctionSpaceElement.hh"

namespace Algorithm
{
  using Interface::AbstractFunctionSpaceElement;

  double ProductSpaceProduct::operator()(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y) const
  {
    if( x.spaceIndex() != y.spaceIndex() ) throw IncompatibleSpaceException("ProductSpaceProduct",x.spaceIndex(),y.spaceIndex());

//    const auto& x_ = toProductSpaceElement(x);
//    const auto& y_ = toProductSpaceElement(y);

//    auto result = 0.;

//    if( x_.isPrimalEnabled() && y_.isPrimalEnabled() )
//      for( auto i : x_.space().primalSubSpaceIds() )
//        result += x_.variable(i)( y_.variable(i) );
//    if( x_.isDualEnabled() && y_.isDualEnabled() )
//      for( auto i : space().dualSubSpaceIds() )
//        result += variable(i)( y_.variable(i) );

    return x(y);
  }
}
