#ifndef ALGORITHM_ADAPTER_KASKADE_L2PRODUCT_HH
#define ALGORITHM_ADAPTER_KASKADE_L2PRODUCT_HH

#include "../../vector.hh"
#include "Util/Exceptions/incompatibleSpaceException.hh"
#include "Util/cast.hh"
#include "vector.hh"

namespace Algorithm
{
  namespace Kaskade
  {
    template <class Description>
    class l2Product
    {
    public:
      double operator()(const ::Algorithm::Vector& x, const ::Algorithm::Vector& y) const
      {
        if( x.spaceIndex() != y.spaceIndex() ) throw IncompatibleSpaceException("l2Product",x.spaceIndex(),y.spaceIndex());

        return cast_ref< Vector<Description> >(x).impl() * cast_ref< Vector<Description> >(y).impl();
      }
    };
  }
}

#endif // ALGORITHM_ADAPTER_KASKADE_L2PRODUCT_HH
