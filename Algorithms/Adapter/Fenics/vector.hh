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

  namespace Fenics
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
      /// Construct vector from underlying vector space.
      explicit Vector(const ::Algorithm::VectorSpace& space);

      /// Assign from dolfin::Function.
      Vector& operator=(const dolfin::Function& v);

//      Vector& axpy(double a, const AbstractVector& y);

      /// Degrees of freedom.
      unsigned size() const;

      /// Access reference to underlying vector.
      dolfin::GenericVector& impl();

      /// Access reference to underlying vector.
      const dolfin::GenericVector& impl() const;

      /// Apply as dual element.
      double operator()(const Vector& y) const;

    private:
      dolfin::Function v_;
    };
  }
}

#endif // ALGORITHM_ADAPTER_FENICS_VECTOR_HH
