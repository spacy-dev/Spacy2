#include "productSpaceProduct.hh"

#include "productSpace.hh"
#include "productSpaceElement.hh"
#include "Util/Exceptions/incompatibleSpaceException.hh"
#include "Util/castTo.hh"

namespace Algorithm
{
  double ProductSpaceProduct::operator()(const Vector& x, const Vector& y) const
  {
    if( x.spaceIndex() != y.spaceIndex() ) throw IncompatibleSpaceException("ProductSpaceProduct",x.spaceIndex(),y.spaceIndex());

    const auto& x_ = castAny<ProductSpaceElement>(x);
    const auto& y_ = castAny<ProductSpaceElement>(y);

    if( x_.isPrimalDualProductSpaceElement() )
    {
      auto result = 0;
      if( x_.isPrimalEnabled() && y_.isPrimalEnabled() ) result += x_.primalComponent() * y_.primalComponent();
      if( x_.isDualEnabled() && y_.isDualEnabled() ) result += x_.dualComponent() * y_.dualComponent();
      x_.reset(y_);
      return result;
    }

    auto result = 0.;
    for( auto i : x_.productSpace().primalSubSpaceIds() )
      result += x_.variable(i)( y_.variable(i) );
    return result;
  }
}
