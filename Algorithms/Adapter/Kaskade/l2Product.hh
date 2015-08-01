#ifndef ALGORITHM_FUNCTION_SPACES_VECTOR_SPACE_L2PRODUCT_HH
#define ALGORITHM_FUNCTION_SPACES_VECTOR_SPACE_L2PRODUCT_HH

#include "../../Interface/abstractFunctionSpaceElement.hh"
#include "../../Interface/abstractDualPairing.hh"
#include "../../Util/incompatiblespaceexception.hh"
#include "../../Util/invalidargumentexception.hh"

#include "vectorSpaceElement.hh"

namespace Algorithm
{
  namespace Kaskade
  {
    template <class Vector>
    class l2Product : public AbstractDualPairing
    {
      double operator()(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y) const final override
      {
        if( x.spaceIndex() != y.spaceIndex() ) throw IncompatibleSpaceException("l2Product",x.spaceIndex(),y.spaceIndex());
        if( dynamic_cast<const ::Algorithm::Kaskade::VectorSpaceElement<Vector>*>(&x) == nullptr ||
            dynamic_cast<const ::Algorithm::Kaskade::VectorSpaceElement<Vector>*>(&y) == nullptr )
          throw InvalidArgumentException("l2Product");

        return dynamic_cast<const ::Algorithm::Kaskade::VectorSpaceElement<Vector>&>(x).impl() * dynamic_cast<const ::Algorithm::Kaskade::VectorSpaceElement<Vector>&>(y).impl();
      }
    };
  }
}

#endif // ALGORITHM_FUNCTION_SPACES_VECTOR_SPACE_L2PRODUCT_HH
