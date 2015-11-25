// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_ADAPTER_GENERIC_VECTOR_HH
#define SPACY_ADAPTER_GENERIC_VECTOR_HH

#include "Spacy/Util/Base/vectorBase.hh"
#include "Spacy/Util/Mixins/impl.hh"
#include "Spacy/Spaces/RealSpace/real.hh"

namespace Spacy
{
  namespace Generic
  {
    /**
     * @ingroup GenericVectorGroup
     * @brief Generic vector implementation for %Rn.
     */
    template <class VectorImpl>
    class Vector :
      public Mixin::Impl<VectorImpl> ,
      public VectorBase ,
      public AddArithmeticOperators< Vector<VectorImpl> >
    {
    public:
      /**
       * @brief Construct zero vector \f$x\f$ from underlying vector space.
       * @param space underlying vector space
       */
      Vector(const VectorSpace& space);

      /**
       * @brief Construct zero vector \f$x\f$ from underlying vector space.
       * @param v inital value
       * @param space underlying vector space
       */
      Vector(const VectorImpl& v, const VectorSpace& space)
        : Mixin::Impl<VectorImpl>( v ),
          VectorBase(space)
      {}

      /**
       * @brief Assign from coefficient vector.
       * @param v coefficient vector
       */
      Vector& operator=(const VectorImpl& v)
      {
        this->impl() = v;
        return *this;
      }

      /**
       * @brief Apply as dual element.
       * @param y primal vector
       * @return \f$x(y)\f$
       */
      Spacy::Real operator()(const Vector& y) const
      {
        return this->impl().dot(y.impl());
      }
    };
  }
}

#endif // SPACY_ADAPTER_GENERIC_VECTOR_HH
