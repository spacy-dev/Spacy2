#ifndef ALGORITHM_ADAPTER_KASKADE_L2PRODUCT_HH
#define ALGORITHM_ADAPTER_KASKADE_L2PRODUCT_HH

#include "Interface/abstractVector.hh"
#include "Interface/abstractScalarProduct.hh"
#include "Util/Exceptions/incompatibleSpaceException.hh"

#include "vector.hh"

namespace Algorithm
{
  namespace Kaskade
  {
    template <class Description>
    class l2Product : public Interface::AbstractScalarProduct
    {
      double operator()(const Interface::AbstractVector& x, const Interface::AbstractVector& y) const final override
      {
        if( x.spaceIndex() != y.spaceIndex() ) throw IncompatibleSpaceException("l2Product",x.spaceIndex(),y.spaceIndex());

        return castTo< Vector<Description> >(x).impl() * castTo< Vector<Description> >(y).impl();
      }
    };
  }
}

#endif // ALGORITHM_ADAPTER_KASKADE_L2PRODUCT_HH
