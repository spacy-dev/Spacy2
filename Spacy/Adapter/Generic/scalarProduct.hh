#pragma once

namespace Spacy
{
  /// @cond
  class Real;
  class Vector;
  /// @endcond

  namespace Generic
  {
    /**
     * @ingroup GenericGroup
     * @brief Generic euclidean scalar product, based on the dual pairing in hilbert spaces.
     */
    class EuclideanScalarProduct
    {
    public:
       Spacy::Real operator()(const ::Spacy::Vector& x, const ::Spacy::Vector& y) const;
    };
  }
}
