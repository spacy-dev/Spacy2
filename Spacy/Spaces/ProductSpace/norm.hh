#ifndef SPACY_SPACES_PRODUCT_SPACE_NORM_HH
#define SPACY_SPACES_PRODUCT_SPACE_NORM_HH

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
     * @brief Canonical norm on product spaces.
     */
    class Norm
    {
    public:
      /// Apply norm product.
      Real operator()(const ::Spacy::Vector& x) const;
    };
  }
}

#endif // SPACY_SPACES_PRODUCT_SPACE_NORM_HH
