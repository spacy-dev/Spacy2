#include "productSpaceProduct.hh"

#include "productSpaceElement.hh"
#include "../../Util/incompatiblespaceexception.hh"
#include "../../Util/invalidargumentexception.hh"
#include "../../Interface/abstractBanachSpace.hh"
#include "../../Interface/abstractFunctionSpaceElement.hh"

namespace Algorithm
{
  double ProductSpaceProduct::operator()(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y) const
  {
    if( x.spaceIndex() != y.spaceIndex() ) throw IncompatibleSpaceException("ProductSpaceProduct",x.spaceIndex(),y.spaceIndex());
    if( !isProductSpaceElement(x) && !isProductSpaceElement(y) ) throw InvalidArgumentException("ProductSpaceProduct");

    const auto& x_ = dynamic_cast<const ProductSpaceElement&>(x);
    const auto& y_ = dynamic_cast<const ProductSpaceElement&>(y);

    auto result = 0.;

    for(auto i=0u; i<x_.size(); ++i)
      result += *x_.variables()[i] * *y_.variables()[i];

    return result;
  }
}
