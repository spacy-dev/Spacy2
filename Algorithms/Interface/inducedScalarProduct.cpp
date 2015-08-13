#include "inducedScalarProduct.hh"

#include "abstractVector.hh"
#include "FunctionSpaces/ProductSpace/productSpace.hh"
#include "FunctionSpaces/ProductSpace/productSpaceElement.hh"

#include "operator.hh"
#include "scalarProduct.hh"
#include "Util/castTo.hh"
#include "Util/create.hh"

namespace Algorithm
{
  namespace Interface
  {
    InducedScalarProduct::InducedScalarProduct(const AbstractOperator& M)
      : Mixin::UniqueImpl<AbstractOperator>(clone(M))
    {}

    double InducedScalarProduct::operator()(const AbstractVector& x, const AbstractVector& y) const
    {
      return (*impl()(y))(x);
    }


    PrimalInducedScalarProduct::PrimalInducedScalarProduct(const AbstractOperator& M)
      : Mixin::UniqueImpl<AbstractOperator>(clone(M))
    {}

    double PrimalInducedScalarProduct::operator()(const AbstractVector& x, const AbstractVector& y) const
    {
      const auto& xx = castTo<ProductSpaceElement>(x);
      const auto& yy = castTo<ProductSpaceElement>(y);

      auto result = 0.;

      if( xx.isDualEnabled() && yy.isDualEnabled() )
        for( auto i : xx.space().dualSubSpaceIds() )
          result += xx.variable(i) * yy.variable(i);

      if( !xx.isPrimalEnabled() || !yy.isPrimalEnabled() )
      {
        xx.reset(yy);
        return result;
      }

      auto x_ = clone(primal(x));
      auto y_ = clone(primal(y));

      return result += (*impl()(*y_))(*x_);
    }
  }

  ScalarProduct inducedScalarProduct(const Operator& A)
  {
    return createFromSharedImpl<ScalarProduct,Interface::InducedScalarProduct>( A.impl() );
  }

  ScalarProduct primalInducedScalarProduct(const Operator& A)
  {
    return createFromSharedImpl<ScalarProduct,Interface::PrimalInducedScalarProduct>( A.impl() );
  }
}
