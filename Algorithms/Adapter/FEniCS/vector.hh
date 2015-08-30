#ifndef ALGORITHM_ADAPTER_FENICS_VECTOR_HH
#define ALGORITHM_ADAPTER_FENICS_VECTOR_HH

#include <dolfin.h>

#include "Algorithms/vector.hh"
#include "Algorithms/Util/Base/vectorBase.hh"
#include "Algorithms/Util/Mixins/impl.hh"


namespace Algorithm
{
  /// @cond
  class VectorSpace;
  /// @endcond

  namespace FEniCS
  {
    /**
     * @ingroup FenicsGroup
     * @brief Vector implementation for FEniCS (single space).
     */
    class Vector :
        public VectorBase<Vector> ,
        public SupportedOperatorBase<Vector>
    {
    public:
      /**
       * @brief Construct vector \f$x\f$ from underlying vector space.
       * @param space underlying vector space
       */
      explicit Vector(const ::Algorithm::VectorSpace& space);

      /**
       * @brief Assign from dolfin::Function.
       * @param v dolfin::Function to assign from
       */
      Vector& operator=(const dolfin::Function& v);

//      Vector& axpy(double a, const AbstractVector& y);

      /**
       * @brief Degrees of freedom.
       * @return number of degrees of freedom (size of coefficient vector)
       */
      unsigned size() const;

      /**
       * @brief Access reference to underlying vector.
       */
      dolfin::GenericVector& impl();

      /**
       * @brief Access reference to underlying vector.
       */
      const dolfin::GenericVector& impl() const;

      /**
       * @brief Apply as dual element.
       * @param y primal vector
       * @return \f$x(y)\f$
       */
      double operator()(const Vector& y) const;

    private:
      dolfin::Function v_;
    };
  }
}

#endif // ALGORITHM_ADAPTER_FENICS_VECTOR_HH
