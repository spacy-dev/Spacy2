// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_SPACES_PRODUCT_SPACE_PRODUCT_HH
#define SPACY_SPACES_PRODUCT_SPACE_PRODUCT_HH

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

#endif // SPACY_SPACES_PRODUCT_SPACE_PRODUCT_HH
