#include "productSpaceProduct.hh"

#include "productSpaceElement.hh"
#include "Util/Exceptions/incompatibleSpaceException.hh"
#include "Util/Exceptions/invalidArgumentException.hh"
#include "Interface/abstractVectorSpace.hh"
#include "Interface/abstractVector.hh"

namespace Algorithm
{
  using Interface::AbstractVector;

  double ProductSpaceProduct::operator()(const AbstractVector& x, const AbstractVector& y) const
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
