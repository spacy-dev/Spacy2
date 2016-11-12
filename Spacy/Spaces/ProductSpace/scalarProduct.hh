#pragma once

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
      /// Apply scalar product.
      Real operator()(const ::Spacy::Vector& x, const ::Spacy::Vector& y) const;
    };
  }
}
