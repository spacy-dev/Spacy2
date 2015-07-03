#include "vector.hh"

#include "Util/Exceptions/invalidArgumentException.hh"

namespace Algorithm
{
  namespace Fenics
  {
    Vector::Vector(const AbstractBanachSpace& space, const dolfin::Function& v)
      : AbstractFunctionSpaceElement(space), Impl<dolfin::Function>(v)
    {}

    void Vector::copyTo(AbstractFunctionSpaceElement& y) const
    {
      toVector(y).impl() = impl();
    }

    void Vector::print(std::ostream& os) const
    {
      //      os << impl(); // todo generalize output
    }

    Vector& Vector::operator=(const dolfin::Function& v)
    {
      impl() = v;
      return *this;
    }

    Vector& Vector::operator=(const AbstractFunctionSpaceElement& y)
    {
      impl() = toVector(y).impl();
      return *this;
    }

    Vector& Vector::operator+=(const AbstractFunctionSpaceElement& y)
    {
      impl() = impl() + toVector(y).impl();
      return *this;
    }

    Vector& Vector::operator-=(const AbstractFunctionSpaceElement& y)
    {
      impl() = impl() - toVector(y).impl();
      return *this;
    }

    Vector& Vector::operator*=(double a)
    {
      *impl().vector() *= a;
      return *this;
    }

    std::unique_ptr<AbstractFunctionSpaceElement> Vector::operator- () const
    {
      auto v = clone( this );
      *v *= -1;
      return std::move(v);
    }

    double& Vector::coefficient(unsigned i)
    {
      return *(impl().vector()->data() + i);
    }

    const double& Vector::coefficient(unsigned i) const
    {
      return *(impl().vector()->data() + i);
    }

    unsigned Vector::size() const
    {
      return impl().vector()->size(); // todo generalize
    }

    dolfin::Function& Vector::operator[](unsigned i)
    {
      return impl()[i];
    }

    const dolfin::Function& Vector::operator[](unsigned i) const
    {
      return impl()[i];
    }

    double Vector::applyAsDualTo(const AbstractFunctionSpaceElement& y) const
    {
      auto const& y_ = toVector(y);
      return impl().vector()->inner( *y_.impl().vector() );
    }

    Vector* Vector::cloneImpl() const
    {
      return new Vector(getSpace(),impl());
    }


    bool isVector(const AbstractFunctionSpaceElement& y)
    {
      return dynamic_cast< const Vector* >(&y) != nullptr;
    }

    Vector& toVector(AbstractFunctionSpaceElement& x)
    {
      if( !isVector(x) ) throw InvalidArgumentException("Vector(const AbstractFunctionSpaceElement&)");
      return dynamic_cast<Vector&>(x);
    }

    const Vector& toVector(const AbstractFunctionSpaceElement& x)
    {
      if( !isVector(x) ) throw InvalidArgumentException("Vector(const AbstractFunctionSpaceElement&)");
      return dynamic_cast<const Vector&>(x);
    }
  }
}
