#ifndef ALGORITHM_ADAPTER_FENICS_VECTOR_HH
#define ALGORITHM_ADAPTER_FENICS_VECTOR_HH

#include <dolfin.h>

#include "../../vector.hh"
#include "../../vectorSpace.hh"
#include "Interface/vectorBase.hh"
#include "Util/Mixins/impl.hh"
#include "Util/Mixins/eps.hh"


namespace Algorithm
{
  namespace Fenics
  {
    class Vector : public VectorBase<Vector>, public Mixin::Impl<dolfin::Function> , public Mixin::Eps
    {
    public:
      explicit Vector(const Algorithm::VectorSpace& space);

      Vector(const dolfin::Function& f, const ::Algorithm::VectorSpace& space);

//      void copyTo(Interface::AbstractVector& y) const;

//      void print(std::ostream& os) const;

      Vector& operator=(const dolfin::Function& v);

      Vector& operator=(const Vector& y);

      Vector& operator+=(const Vector& y);

//      Vector& axpy(double a, const AbstractVector& y);

      Vector& operator-=(const Vector& y);

      Vector& operator*=(double a);

      Vector operator- () const;

      bool operator==(const Vector& y) const;

      double& coefficient(unsigned i);

      const double& coefficient(unsigned i) const;

      unsigned size() const;

      dolfin::Function& operator[](unsigned i);

      const dolfin::Function& operator[](unsigned i) const;

      double operator()(const Vector& y) const;
//    private:

//      Vector* cloneImpl() const;
    };


    void copy(const ::Algorithm::Vector& x, dolfin::GenericVector& y, bool verbose = false);

    void copy(const ::Algorithm::Vector& x, dolfin::Function& y, bool verbose = false);

    void copy(const dolfin::GenericVector& y, ::Algorithm::Vector& x, bool verbose = false);
  }
}

#endif // ALGORITHM_ADAPTER_FENICS_VECTOR_HH
