// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_ADAPTER_GENERIC_SCALAR_PRODUCT_HH
#define SPACY_ADAPTER_GENERIC_SCALAR_PRODUCT_HH

namespace Spacy
{
  /// @cond
  class Real;
  class Vector;
  /// @endcond

  namespace Generic
  {
    /// Generic euclidean scalar product, based on the dual pairing in hilbert spaces.
    class EuclideanScalarProduct
    {
    public:
       Spacy::Real operator()(const ::Spacy::Vector& x, const ::Spacy::Vector& y) const;
    };
  }
}


#endif // SPACY_ADAPTER_GENERIC_SCALAR_PRODUCT_HH

