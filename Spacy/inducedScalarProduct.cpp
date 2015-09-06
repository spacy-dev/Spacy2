#include "inducedScalarProduct.hh"

#include "Spacy/Spaces/productSpace.hh"
#include "Spacy/Spaces/RealSpace/real.hh"
#include "Util/cast.hh"

#include <utility>

namespace Spacy
{
  InducedScalarProduct::InducedScalarProduct(CallableOperator M)
    : M_(std::move(M))
  {}

  Real InducedScalarProduct::operator()(const Vector& x, const Vector& y) const
  {
    return M_(y)(x);
  }


  PrimalInducedScalarProduct::PrimalInducedScalarProduct(CallableOperator M)
    : M_(std::move(M))
  {}

  Real PrimalInducedScalarProduct::operator()(const Vector& x, const Vector& y) const
  {
    const auto& xx = cast_ref<ProductSpace::Vector>(x);
    const auto& yy = cast_ref<ProductSpace::Vector>(y);

    auto result = Real{0.};

//    if( xx.isDualEnabled() && yy.isDualEnabled() )
//      for( auto i : xx.creator().dualSubSpaceIds() )
//        result += xx.variable(i) * yy.variable(i);

    if( !xx.isPrimalEnabled() || !yy.isPrimalEnabled() )
    {
      xx.reset(yy);
      return result;
    }

    auto x_ = primal(x);
    auto y_ = primal(y);

    result += M_(y_)(x_);

    return result;
  }
}
