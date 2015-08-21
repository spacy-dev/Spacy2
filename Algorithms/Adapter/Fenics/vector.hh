#ifndef ALGORITHM_ADAPTER_FENICS_VECTOR_HH
#define ALGORITHM_ADAPTER_FENICS_VECTOR_HH

#include <dolfin.h>

#include "../../vector.hh"
#include "../../vectorSpace.hh"
#include "Interface/abstractVector.hh"
#include "Util/Mixins/impl.hh"


namespace Algorithm
{
  namespace Fenics
  {
    class Vector : public Interface::AbstractVector, public Mixin::Impl<dolfin::Function>
    {
    public:
      explicit Vector(const Algorithm::VectorSpace& space);

      Vector(const dolfin::Function& f, const ::Algorithm::VectorSpace& space);

      void copyTo(Interface::AbstractVector& y) const final override;

      void print(std::ostream& os) const final override;

      Vector& operator=(const dolfin::Function& v);

      Vector& operator=(const Interface::AbstractVector& y) final override;

      Vector& operator+=(const Interface::AbstractVector& y) final override;

      Vector& axpy(double a, const AbstractVector& y) final override;

      Vector& operator-=(const Interface::AbstractVector& y) final override;

      Vector& operator*=(double a) final override;

      std::unique_ptr<Interface::AbstractVector> operator- () const final override;

      double& coefficient(unsigned i) final override;

      const double& coefficient(unsigned i) const final override;

      unsigned size() const;

      dolfin::Function& operator[](unsigned i);

      const dolfin::Function& operator[](unsigned i) const;

    private:
      double applyAsDualTo(const Interface::AbstractVector& y) const final override;

      Vector* cloneImpl() const final override;
    };


    void copy(const Interface::AbstractVector& x, dolfin::GenericVector& y, bool verbose = false);

    void copy(const ::Algorithm::Vector& x, dolfin::Function& y, bool verbose = false);

    void copy(const dolfin::GenericVector& y, Interface::AbstractVector& x, bool verbose = false);
  }
}

#endif // ALGORITHM_ADAPTER_FENICS_VECTOR_HH
