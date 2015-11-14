#ifndef SPACY_PRIMAL_DUAL_PRODUCT_SPACE_PRODUCT_HH
#define SPACY_PRIMAL_DUAL_PRODUCT_SPACE_PRODUCT_HH

namespace Spacy
{
  /// @cond
  class Real;
  class Vector;
  /// @endcond

  namespace PrimalDualProductSpace
  {
    /**
     * @ingroup PrimalDualProductSpaceGroup
     * @brief Canonical scalar product on product spaces.
     */
    class ScalarProduct
    {
    public:
      /// Apply scalar product.
      Real operator()(const ::Spacy::Vector& x, const ::Spacy::Vector& y) const;
    };
  }
}

#endif // SPACY_PRIMAL_DUAL_PRODUCT_SPACE_PRODUCT_HH
