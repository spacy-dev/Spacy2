#include "inducedScalarProduct.hh"

#include "FunctionSpaces/ProductSpace/productSpace.hh"
#include "FunctionSpaces/ProductSpace/productSpaceElement.hh"

#include "operator.hh"
#include "scalarProduct.hh"
#include "Util/castTo.hh"
#include "Util/create.hh"

namespace Algorithm
{
//  namespace Interface
//  {
    InducedScalarProduct::InducedScalarProduct(const Interface::AbstractOperator& M)
      : Mixin::UniqueImpl<Interface::AbstractOperator>(clone(M))
    {}

    double InducedScalarProduct::operator()(const Vector& x, const Vector& y) const
    {
      return impl()(y)(x);
    }


    PrimalInducedScalarProduct::PrimalInducedScalarProduct(const Interface::AbstractOperator& M)
      : Mixin::UniqueImpl<Interface::AbstractOperator>(clone(M))
    {}

    double PrimalInducedScalarProduct::operator()(const Vector& x, const Vector& y) const
    {
      const auto& xx = castAny<ProductSpaceElement>(x);
      const auto& yy = castAny<ProductSpaceElement>(y);

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

      return result += impl()(y_)(x_);
    }
//  }

  InducedScalarProduct inducedScalarProduct(const Operator& A)
  {
    return InducedScalarProduct(A.impl());
//    return createFromSharedImpl<ScalarProduct,Interface::InducedScalarProduct>( A.impl() );
  }

  PrimalInducedScalarProduct primalInducedScalarProduct(const Operator& A)
  {
    return PrimalInducedScalarProduct(A.impl());
//    return createFromSharedImpl<ScalarProduct,Interface::PrimalInducedScalarProduct>( A.impl() );
  }
}
