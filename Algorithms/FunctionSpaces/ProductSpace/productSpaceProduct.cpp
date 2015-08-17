#include "productSpaceProduct.hh"

#include "productSpace.hh"
#include "productSpaceElement.hh"
#include "Util/Exceptions/incompatibleSpaceException.hh"
#include "Interface/abstractVector.hh"

namespace Algorithm
{
  double ProductSpaceProduct::operator()(const Interface::AbstractVector& x, const Interface::AbstractVector& y) const
  {
    if( x.spaceIndex() != y.spaceIndex() ) throw IncompatibleSpaceException("ProductSpaceProduct",x.spaceIndex(),y.spaceIndex());

    const auto& x_ = castTo<ProductSpaceElement>(x);
    const auto& y_ = castTo<ProductSpaceElement>(y);

    if( x_.isPrimalDualProductSpaceElement() )
    {
      auto result = 0;
      if( x_.isPrimalEnabled() && y_.isPrimalEnabled() ) result += x_.primalComponent() * y_.primalComponent();
      if( x_.isDualEnabled() && y_.isDualEnabled() ) result += x_.dualComponent() * y_.dualComponent();
      x_.reset(y_);
      return result;
    }

    auto result = 0.;
    for( auto i : x_.space().primalSubSpaceIds() )
      result += x_.variable(i)( y_.variable(i) );
    return result;
  }
}
