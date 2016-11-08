#pragma once

namespace Spacy
{
  /// @cond
  class Real;
  class Vector;
  class VectorSpace;
  /// @endcond

  namespace Kaskade
  {
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
