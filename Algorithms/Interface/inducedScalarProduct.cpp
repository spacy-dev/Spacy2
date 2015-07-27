#include "inducedScalarProduct.hh"

#include "abstractFunctionSpaceElement.hh"
#include "FunctionSpaces/ProductSpace/productSpaceElement.hh"

#include "operator.hh"
#include "scalarProduct.hh"
#include "Util/create.hh"

namespace Algorithm
{
  namespace Interface
  {
    InducedScalarProduct::InducedScalarProduct(const AbstractOperator& M)
      : Mixin::UniqueImpl<AbstractOperator>(clone(M))
    {}

    double InducedScalarProduct::operator()(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y) const
    {
      return (*impl()(y))(x);
    }


    PrimalInducedScalarProduct::PrimalInducedScalarProduct(const AbstractOperator& M)
      : Mixin::UniqueImpl<AbstractOperator>(clone(M))
    {}

    double PrimalInducedScalarProduct::operator()(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y) const
    {
      auto x_ = clone(primal(x));
      auto y_ = clone(primal(y));

      auto result = (*impl()(*y_))(*x_);

      const auto& xx = toProductSpaceElement(x);
      const auto& yy = toProductSpaceElement(y);

      for( auto i : xx.space().dualSubSpaceIds() )
        result += xx.variable(i) * yy.variable(i);

      return result;
      //      return (*impl()(y))(x);
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
