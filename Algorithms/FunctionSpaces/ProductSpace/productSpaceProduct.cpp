#include "productSpaceProduct.hh"

#include "productSpaceElement.hh"
#include "../../Util/incompatiblespaceexception.hh"
#include "../../Util/invalidargumentexception.hh"
#include "../../Interface/abstractHilbertSpace.hh"

namespace Algorithm
{
  double ProductSpaceProduct::operator()(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y) const
  {
    if( x.spaceIndex() != y.spaceIndex() ) throw IncompatibleSpaceException("ProductSpaceProduct",x.spaceIndex(),y.spaceIndex());
    if( !isProductSpaceElement(x) && !isProductSpaceElement(y) ) throw InvalidArgumentException("ProductSpaceProduct");

    const auto& x_ = dynamic_cast<const ProductSpaceElement&>(x);
    const auto& y_ = dynamic_cast<const ProductSpaceElement&>(y);

    auto result = 0.;

    if( !x_.disablePrimal_ && !y_.disablePrimal_ )
      for(auto i=0u; i<x_.primalVariables_.size(); ++i)
        result += *x_.primalVariables_[i] * *y_.primalVariables_[i];

    if( !x_.disableDual_ && !y_.disableDual_ )
      for(auto i=0u; i<x_.dualVariables_.size(); ++i)
        result += *x_.dualVariables_[i] * *y_.dualVariables_[i];

    x_.reset();
    y_.reset();

    return result;
  }
}
