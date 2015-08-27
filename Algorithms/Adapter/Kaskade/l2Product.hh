#ifndef ALGORITHM_ADAPTER_KASKADE_L2PRODUCT_HH
#define ALGORITHM_ADAPTER_KASKADE_L2PRODUCT_HH

#include "../../vector.hh"
#include "Util/Exceptions/incompatibleSpaceException.hh"
#include "Util/cast.hh"

namespace Algorithm
{
  namespace Kaskade
  {
    /// \cond
    template <class> class Vector;
    /// \endcond

    /**
     * @ingroup KaskadeGroup
     * @brief l2 scalar product for Kaskade 7.
     */
    template <class Description>
    class l2Product
    {
    public:
      /// Compute \f$ (x,y) = \sum_i x_i y_i \f$.
      double operator()(const ::Algorithm::Vector& x, const ::Algorithm::Vector& y) const
      {
        if( x.space()->index() != y.space()->index() )
          throw IncompatibleSpaceException("Kaskade::l2Product",x.space()->index(),y.space()->index());

        return cast_ref< Vector<Description> >(x).impl() * cast_ref< Vector<Description> >(y).impl();
      }
    };
  }
}

#endif // ALGORITHM_ADAPTER_KASKADE_L2PRODUCT_HH
