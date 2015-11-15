#ifndef SPACY_ADAPTER_FENICS_L2_PRODUCT_HH
#define SPACY_ADAPTER_FENICS_L2_PRODUCT_HH

namespace Spacy
{
  /// @cond
  class Real;
  class Vector;
  /// @endcond
  namespace FEniCS
  {
    /**
     * @ingroup FenicsGroup
     * @brief l2-product \f$(x,y) = \sum_i x_i y_i \f$ for %FEniCS.
     */
    class l2Product
    {
    public:
      /// Apply l2-product.
      Real operator()(const ::Spacy::Vector& x, const ::Spacy::Vector& y) const;
    };
  }
}

#endif // SPACY_ADAPTER_FENICS_L2_PRODUCT_HH

