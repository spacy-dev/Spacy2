// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_ADAPTER_GENERIC_VECTOR_CREATOR_HH
#define SPACY_ADAPTER_GENERIC_VECTOR_CREATOR_HH

#include "vector.hh"
#include "Spacy/vectorSpace.hh"

namespace Spacy
{
  namespace Generic
  {
    /**
     * @ingroup GenericGroup
     * @brief Generic vector creator implementation
     * @todo generalize vector creation
     */
    template <class VectorImpl>
    class VectorCreator
    {
    public:
      explicit VectorCreator(unsigned dim) : dim_(dim)
      {}

      Vector<VectorImpl> operator()(const VectorSpace* space) const
      {
        return Vector<VectorImpl>(VectorImpl::Zero(dim_),*space);
      }

      unsigned dim() const
      {
        return dim_;
      }

    private:
      unsigned dim_;
    };
  }
}

#endif // SPACY_ADAPTER_GENERIC_VECTOR_CREATOR_HH

