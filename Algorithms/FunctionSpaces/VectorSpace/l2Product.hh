#ifndef ALGORITHM_FUNCTION_SPACES_VECTOR_SPACE_L2PRODUCT_HH
#define ALGORITHM_FUNCTION_SPACES_VECTOR_SPACE_L2PRODUCT_HH

#include "Interface/abstractFunctionSpaceElement.hh"
#include "Interface/abstractScalarProduct.hh"
#include "Util/Exceptions/incompatibleSpaceException.hh"
#include "Util/Exceptions/invalidArgumentException.hh"

#include "vectorSpaceElement.hh"

namespace Algorithm
{
  template <class Vector>
  class l2Product : public AbstractScalarProduct
  {
    double operator()(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y) const final override
    {
      if( x.spaceIndex() != y.spaceIndex() ) throw IncompatibleSpaceException("l2Product",x.spaceIndex(),y.spaceIndex());
      if( dynamic_cast<const VectorSpaceElement<Vector>*>(&x) == nullptr || dynamic_cast<const VectorSpaceElement<Vector>*>(&y) == nullptr ) throw InvalidArgumentException("l2Product");

      double result = dot( dynamic_cast<const VectorSpaceElement<Vector>&>(x).impl() , dynamic_cast<const VectorSpaceElement<Vector>&>(y).impl() );
      return result;
//      auto result = 0.;
//      for(unsigned int i=0; i<x.size(); ++i) result += x.coefficient(i) * y.coefficient(i);
//      return result;
    }
  };
}

#endif // ALGORITHM_FUNCTION_SPACES_VECTOR_SPACE_L2PRODUCT_HH
