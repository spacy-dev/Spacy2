#include "inducedScalarProduct.hh"

#include "FunctionSpaces/productSpace.hh"
#include "Util/cast.hh"

#include <utility>

namespace Algorithm
{
  InducedScalarProduct::InducedScalarProduct(CallableOperator M)
    : M_(std::move(M))
  {}

  double InducedScalarProduct::operator()(const Vector& x, const Vector& y) const
  {
    return M_(y)(x);
  }


  PrimalInducedScalarProduct::PrimalInducedScalarProduct(CallableOperator M)
    : M_(std::move(M))
  {}

  double PrimalInducedScalarProduct::operator()(const Vector& x, const Vector& y) const
  {
    const auto& xx = cast_ref<ProductSpace::Vector>(x);
    const auto& yy = cast_ref<ProductSpace::Vector>(y);

    auto result = 0.;

    if( xx.isDualEnabled() && yy.isDualEnabled() )
      for( auto i : xx.productSpace().dualSubSpaceIds() )
        result += xx.variable(i) * yy.variable(i);

    if( !xx.isPrimalEnabled() || !yy.isPrimalEnabled() )
    {
      xx.reset(yy);
      return result;
    }

    auto x_ = primal(x);
    auto y_ = primal(y);

    return result += M_(y_)(x_);
  }
}
