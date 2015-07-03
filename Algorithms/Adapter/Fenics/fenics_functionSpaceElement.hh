#ifndef ALGORITHM_FUNCTION_SPACES_FENICS_VECTOR_SPACE_ELEMENT_HH
#define ALGORITHM_FUNCTION_SPACES_FENICS_VECTOR_SPACE_ELEMENT_HH

#include "Interface/abstractFunctionSpaceElement.hh"
#include "Util/Mixins/impl.hh"

#include <dolfin.h>

namespace Algorithm
{
  class AbstractBanachSpace;

  class Fenics_Vector : public AbstractFunctionSpaceElement, public Mixin::Impl<dolfin::Function>
  {
  public:
    Fenics_Vector(const AbstractBanachSpace& space, const dolfin::Function& v);

    void copyTo(AbstractFunctionSpaceElement& y) const final override;

    void print(std::ostream& os) const final override;

    Fenics_Vector& operator=(const dolfin::Function& v);

    Fenics_Vector& operator=(const AbstractFunctionSpaceElement& y) final override;

    Fenics_Vector& operator+=(const AbstractFunctionSpaceElement& y) final override;

    Fenics_Vector& operator-=(const AbstractFunctionSpaceElement& y) final override;

    Fenics_Vector& operator*=(double a) final override;

    std::unique_ptr<AbstractFunctionSpaceElement> operator- () const final override;

    double& coefficient(unsigned i) final override;

    const double& coefficient(unsigned i) const final override;

    unsigned size() const;

    dolfin::Function& operator[](unsigned i);

    const dolfin::Function& operator[](unsigned i) const;

  private:
    double applyAsDualTo(const AbstractFunctionSpaceElement& y) const final override;

    Fenics_Vector* cloneImpl() const final override;
  };

  bool isFenicsVector(const AbstractFunctionSpaceElement& y);

  Fenics_Vector& fenics_Vector(AbstractFunctionSpaceElement& x);

  const Fenics_Vector& fenics_Vector(const AbstractFunctionSpaceElement& x);
}

#endif // ALGORITHM_FUNCTION_SPACES_FENICS_VECTOR_SPACE_ELEMENT_HH
