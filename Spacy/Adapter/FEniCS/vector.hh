// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_ADAPTER_FENICS_VECTOR_HH
#define SPACY_ADAPTER_FENICS_VECTOR_HH

#include <dolfin.h>

#include "Spacy/Util/Base/addArithmeticOperators.hh"
#include "Spacy/Util/Base/vectorBase.hh"


namespace Spacy
{
  /// @cond
  class Real;
  class VectorSpace;
  /// @endcond

  namespace FEniCS
  {
    /**
     * @ingroup FenicsGroup
     * @brief %Vector implementation for %FEniCS (single space).
     */
    class Vector :
        public VectorBase ,
        public AddArithmeticOperators<Vector>
    {
    public:
      /**
       * @brief Construct vector \f$x\f$ from underlying vector space.
       * @param space underlying vector space
       */
      explicit Vector(const VectorSpace& space);

      /**
       * @brief Assign from dolfin::Function.
       * @param v dolfin::Function to assign from
       */
      Vector& operator=(const dolfin::Function& v);

//      Vector& axpy(double a, const AbstractVector& y);

      /**
       * @brief Access reference to underlying vector.
       */
      dolfin::GenericVector& get();

      /**
       * @brief Access reference to underlying vector.
       */
      const dolfin::GenericVector& get() const;

      /**
       * @brief Apply as dual element.
       * @param y primal vector
       * @return \f$x(y)\f$
       */
      Real operator()(const Vector& y) const;

    private:
      dolfin::Function v_;
    };
  }
}

#endif // SPACY_ADAPTER_FENICS_VECTOR_HH
