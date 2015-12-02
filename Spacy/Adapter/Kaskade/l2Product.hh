// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_ADAPTER_KASKADE_L2PRODUCT_HH
#define SPACY_ADAPTER_KASKADE_L2PRODUCT_HH

namespace Spacy
{
  /// @cond
  class Real;
  class Vector;
  class VectorSpace;
  /// @endcond

  namespace Kaskade
  {
    /// @cond
    template <class> class Vector;
    /// @endcond

    /**
     * @ingroup KaskadeGroup
     * @brief Generic l2 scalar product for Kaskade7. Based on the implementation of the dual pairing.
     */
    template <class Description>
    class l2Product
    {
    public:
      /**
       * @brief Compute l2 product.
       * @param x vector
       * @param y vector
       * @return \f$(x,y) = \sum_i x_i y_i \f$.
       */
      Real operator()(const ::Spacy::Vector& x, const ::Spacy::Vector& y) const
      {
        checkSpaceCompatibility(x.space(),y.space());
        return x(y);
      }
    };
  }
}

#endif // SPACY_ADAPTER_KASKADE_L2PRODUCT_HH
