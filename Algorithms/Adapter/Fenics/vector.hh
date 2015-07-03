#ifndef ALGORITHM_FUNCTION_SPACES_FENICS_VECTOR_SPACE_ELEMENT_HH
#define ALGORITHM_FUNCTION_SPACES_FENICS_VECTOR_SPACE_ELEMENT_HH

#include "Interface/abstractFunctionSpaceElement.hh"
#include "Util/Mixins/impl.hh"

#include <dolfin.h>

namespace Algorithm
{
  class AbstractBanachSpace;

  namespace Fenics
  {
    class Vector : public AbstractFunctionSpaceElement, public Mixin::Impl<dolfin::Function>
    {
    public:
      Vector(const AbstractBanachSpace& space, const dolfin::Function& v);

      void copyTo(AbstractFunctionSpaceElement& y) const final override;

      void print(std::ostream& os) const final override;

      Vector& operator=(const dolfin::Function& v);

      Vector& operator=(const AbstractFunctionSpaceElement& y) final override;

      Vector& operator+=(const AbstractFunctionSpaceElement& y) final override;

      Vector& operator-=(const AbstractFunctionSpaceElement& y) final override;

      Vector& operator*=(double a) final override;

      std::unique_ptr<AbstractFunctionSpaceElement> operator- () const final override;

      double& coefficient(unsigned i) final override;

      const double& coefficient(unsigned i) const final override;

      unsigned size() const;

      dolfin::Function& operator[](unsigned i);

      const dolfin::Function& operator[](unsigned i) const;

    private:
      double applyAsDualTo(const AbstractFunctionSpaceElement& y) const final override;

      Vector* cloneImpl() const final override;
    };

    bool isVector(const AbstractFunctionSpaceElement& y);

    Vector& toVector(AbstractFunctionSpaceElement& x);

    const Vector& toVector(const AbstractFunctionSpaceElement& x);
  }
}

#endif // ALGORITHM_FUNCTION_SPACES_FENICS_VECTOR_SPACE_ELEMENT_HH
