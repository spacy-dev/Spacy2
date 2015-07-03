#include "fenics_functionSpaceElement.hh"

#include "Util/Exceptions/invalidArgumentException.hh"

namespace Algorithm
{
  Fenics_Vector::Fenics_Vector(const AbstractBanachSpace& space, const dolfin::Function& v)
    : AbstractFunctionSpaceElement(space), Impl<dolfin::Function>(v)
  {}

  void Fenics_Vector::copyTo(AbstractFunctionSpaceElement& y) const
  {
    fenics_Vector(y).impl() = impl();
  }

  void Fenics_Vector::print(std::ostream& os) const
  {
    //      os << impl(); // todo generalize output
  }

  Fenics_Vector& Fenics_Vector::operator=(const dolfin::Function& v)
  {
    impl() = v;
    return *this;
  }

  Fenics_Vector& Fenics_Vector::operator=(const AbstractFunctionSpaceElement& y)
  {
    impl() = fenics_Vector(y).impl();
    return *this;
  }

  Fenics_Vector& Fenics_Vector::operator+=(const AbstractFunctionSpaceElement& y)
  {
    impl() = impl() + fenics_Vector(y).impl();
    return *this;
  }

  Fenics_Vector& Fenics_Vector::operator-=(const AbstractFunctionSpaceElement& y)
  {
    impl() = impl() - fenics_Vector(y).impl();
    return *this;
  }

  Fenics_Vector& Fenics_Vector::operator*=(double a)
  {
    *impl().vector() *= a;
    return *this;
  }

  std::unique_ptr<AbstractFunctionSpaceElement> Fenics_Vector::operator- () const
  {
    auto v = clone( this );
    *v *= -1;
    return std::move(v);
  }

  double& Fenics_Vector::coefficient(unsigned i)
  {
    return *(impl().vector()->data() + i);
  }

  const double& Fenics_Vector::coefficient(unsigned i) const
  {
    return *(impl().vector()->data() + i);
  }

  unsigned Fenics_Vector::size() const
  {
    return impl().vector()->size(); // todo generalize
  }

  dolfin::Function& Fenics_Vector::operator[](unsigned i)
  {
    return impl()[i];
  }

  const dolfin::Function& Fenics_Vector::operator[](unsigned i) const
  {
    return impl()[i];
  }

  double Fenics_Vector::applyAsDualTo(const AbstractFunctionSpaceElement& y) const
  {
    auto const& y_ = fenics_Vector(y);
    return impl().vector()->inner( *y_.impl().vector() );
  }

  Fenics_Vector* Fenics_Vector::cloneImpl() const
  {
    return new Fenics_Vector(getSpace(),impl());
  }


  bool isFenicsVector(const AbstractFunctionSpaceElement& y)
  {
    return dynamic_cast< const Fenics_Vector* >(&y) != nullptr;
  }

  Fenics_Vector& fenics_Vector(AbstractFunctionSpaceElement& x)
  {
    if( !isFenicsVector(x) ) throw InvalidArgumentException("fenics_Vector(const AbstractFunctionSpaceElement&)");
    return dynamic_cast<Fenics_Vector&>(x);
  }

  const Fenics_Vector& fenics_Vector(const AbstractFunctionSpaceElement& x)
  {
    if( !isFenicsVector(x) ) throw InvalidArgumentException("fenics_Vector(const AbstractFunctionSpaceElement&)");
    return dynamic_cast<const Fenics_Vector&>(x);
  }
}
