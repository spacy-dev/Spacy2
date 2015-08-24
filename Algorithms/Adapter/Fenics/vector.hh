#ifndef ALGORITHM_ADAPTER_FENICS_VECTOR_HH
#define ALGORITHM_ADAPTER_FENICS_VECTOR_HH

#include <dolfin.h>

#include "../../vector.hh"
#include "vectorSpace.hh"
#include "Util/Base/vectorBase.hh"
#include "Util/Mixins/impl.hh"


namespace Algorithm
{
  namespace Fenics
  {
    class Vector :
        public VectorBase<Vector> ,
        public SupportedOperatorBase<Vector>
    {
    public:
      explicit Vector(const ::Algorithm::VectorSpace& space);

      Vector(const dolfin::Function& f, const ::Algorithm::VectorSpace& space);

      Vector& operator=(const dolfin::Function& v);

      Vector& operator=(const Vector& y);

//      Vector& axpy(double a, const AbstractVector& y);

      unsigned size() const;

      dolfin::GenericVector& impl();

      const dolfin::GenericVector& impl() const;

      double operator()(const Vector& y) const;

    private:
      dolfin::Function v_;
    };
  }
}

#endif // ALGORITHM_ADAPTER_FENICS_VECTOR_HH
