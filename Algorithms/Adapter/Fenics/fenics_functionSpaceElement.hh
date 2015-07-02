#ifndef ALGORITHM_FUNCTION_SPACES_FENICS_VECTOR_SPACE_ELEMENT_HH
#define ALGORITHM_FUNCTION_SPACES_FENICS_VECTOR_SPACE_ELEMENT_HH

#include <memory>
#include <vector>
#include <utility>

#include "Interface/abstractFunctionSpaceElement.hh"
#include "Util/invalidargumentexception.hh"
#include "Util/impl.hh"

#include <dolfin.h>

namespace Algorithm
{  
  class Fenics_Vector;
  bool isFenicsVector(const AbstractFunctionSpaceElement&);
  Fenics_Vector& fenics_Vector(AbstractFunctionSpaceElement& x);
  const Fenics_Vector& fenics_Vector(const AbstractFunctionSpaceElement& x);

  class Fenics_Vector : public AbstractFunctionSpaceElement, public Impl<dolfin::Function>
  {
  public:
    Fenics_Vector(const AbstractBanachSpace& space, const dolfin::Function& v)
      : AbstractFunctionSpaceElement(space), Impl<dolfin::Function>(v)
    {}

//    explicit Fenics_Vector(const AbstractBanachSpace& space)
//      : AbstractFunctionSpaceElement(space), impl()()// todo: generalize init
//    {}

    void copyTo(AbstractFunctionSpaceElement& y) const override
    {
      fenics_Vector(y).impl() = impl();
    }

    void print(std::ostream& os) const final override
    {
      //      os << impl(); // todo generalize output
    }

    Fenics_Vector& operator=(const dolfin::Function& v)
    {
      impl() = v;
      return *this;
    }

    Fenics_Vector& operator=(const AbstractFunctionSpaceElement& y) final override
    {
      impl() = fenics_Vector(y).impl();
      return *this;
    }

    Fenics_Vector& operator+=(const AbstractFunctionSpaceElement& y) final override
    {
      impl() = impl() + fenics_Vector(y).impl();
      return *this;
    }

    Fenics_Vector& operator-=(const AbstractFunctionSpaceElement& y) final override
    {
      impl() = impl() - fenics_Vector(y).impl();
      return *this;
    }

    Fenics_Vector& operator*=(double a) final override
    {
      *impl().vector() *= a; // todo generalize
      return *this;
    }

    std::unique_ptr<AbstractFunctionSpaceElement> operator- () const final override
    {
      auto v = clone( this );
      *v *= -1;
      return std::move(v);
    }

    double& coefficient(unsigned i) final override
    {
      return *(impl().vector()->data() + i);
    }

    const double& coefficient(unsigned i) const final override
    {
      return *(impl().vector()->data() + i);
    }

    unsigned size() const
    {
      return impl().vector()->size(); // todo generalize
    }

  private:
    double applyAsDualTo(const AbstractFunctionSpaceElement& y) const final override
    {
      auto const& y_ = fenics_Vector(y);
      return impl().vector()->inner( *y_.impl().vector() );
    }

    Fenics_Vector* cloneImpl() const final override
    {
      return new Fenics_Vector(getSpace(),impl());
    }
  };

  inline bool isFenicsVector(const AbstractFunctionSpaceElement& y)
  {
    return dynamic_cast< const Fenics_Vector* >(&y) != nullptr;
  }

  inline Fenics_Vector& fenics_Vector(AbstractFunctionSpaceElement& x)
  {
    if( !isFenicsVector(x) ) throw InvalidArgumentException("fenics_Vector(const AbstractFunctionSpaceElement&)");
    return dynamic_cast<Fenics_Vector&>(x);
  }

  inline const Fenics_Vector& fenics_Vector(const AbstractFunctionSpaceElement& x)
  {
    if( !isFenicsVector(x) ) throw InvalidArgumentException("fenics_Vector(const AbstractFunctionSpaceElement&)");
    return dynamic_cast<const Fenics_Vector&>(x);
  }
}

#endif // ALGORITHM_FUNCTION_SPACES_FENICS_VECTOR_SPACE_ELEMENT_HH
