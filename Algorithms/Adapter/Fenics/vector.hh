#ifndef ALGORITHM_FUNCTION_SPACES_FENICS_VECTOR_SPACE_ELEMENT_HH
#define ALGORITHM_FUNCTION_SPACES_FENICS_VECTOR_SPACE_ELEMENT_HH

#include "Interface/abstractFunctionSpaceElement.hh"
#include "Util/Mixins/impl.hh"

#include <dolfin.h>

namespace Algorithm
{
  namespace Interface { class AbstractBanachSpace; }

  namespace Fenics
  {
    class Vector : public Interface::AbstractFunctionSpaceElement, public Mixin::Impl<dolfin::Function>
    {
    public:
      Vector(const Interface::AbstractBanachSpace& space, const dolfin::Function& v);

      void copyTo(Interface::AbstractFunctionSpaceElement& y) const final override;

      void print(std::ostream& os) const final override;

      Vector& operator=(const dolfin::Function& v);

      Vector& operator=(const Interface::AbstractFunctionSpaceElement& y) final override;

      Vector& operator+=(const Interface::AbstractFunctionSpaceElement& y) final override;

      Vector& operator-=(const Interface::AbstractFunctionSpaceElement& y) final override;

      Vector& operator*=(double a) final override;

      std::unique_ptr<Interface::AbstractFunctionSpaceElement> operator- () const final override;

      double& coefficient(unsigned i) final override;

      const double& coefficient(unsigned i) const final override;

      unsigned size() const;

      dolfin::Function& operator[](unsigned i);

      const dolfin::Function& operator[](unsigned i) const;

    private:
      double applyAsDualTo(const Interface::AbstractFunctionSpaceElement& y) const final override;

      Vector* cloneImpl() const final override;
    };

    bool isVector(const Interface::AbstractFunctionSpaceElement& y);

    Vector& toVector(Interface::AbstractFunctionSpaceElement& x);

    const Vector& toVector(const Interface::AbstractFunctionSpaceElement& x);
  }
}

#endif // ALGORITHM_FUNCTION_SPACES_FENICS_VECTOR_SPACE_ELEMENT_HH
