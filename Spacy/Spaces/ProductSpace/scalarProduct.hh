#ifndef SPACY_PRODUCT_SPACE_PRODUCT_HH
#define SPACY_PRODUCT_SPACE_PRODUCT_HH

namespace Spacy
{
  /// @cond
  class Real;
  class Vector;
  /// @endcond

  namespace ProductSpace
  {
    /**
     * @ingroup ProductSpaceGroup
     * @brief Canonical scalar product on product spaces.
     */
    class ScalarProduct
    {
    public:
      /**
       * @brief Compute l2 product.
       * @param x vector (ProductSpace::Vector)
       * @param y vector (ProductSpace::Vector)
       * @return \f$(x,y) = \sum_i x_i y_i \f$.
       */
      Real operator()(const ::Spacy::Vector& x, const ::Spacy::Vector& y) const;
    };
  }
}

#endif // SPACY_PRODUCT_SPACE_PRODUCT_HH
