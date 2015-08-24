#include "vector.hh"

#include "../../vectorSpace.hh"

#include "Util/cast.hh"
#include "Util/Exceptions/callOfUndefinedFunctionException.hh"
#include "Util/Exceptions/invalidArgumentException.hh"

#include <utility>

namespace Algorithm
{
  namespace Fenics
  {
    Vector::Vector(const ::Algorithm::VectorSpace& space)
      : Vector( dolfin::Function( cast_ref<VectorCreator>(space.impl()).impl() ) , space )
    {}

    Vector::Vector(const dolfin::Function& f, const ::Algorithm::VectorSpace& space)
      : VectorBase<Vector>(space), Impl<dolfin::Function>( f )
    {}

    Vector& Vector::operator=(const dolfin::Function& v)
    {
      impl() = v;
      return *this;
    }

    Vector& Vector::operator=(const Vector& y)
    {
      *impl().vector() = *y.impl().vector();
      return *this;
    }

    Vector& Vector::operator+=(const Vector& y)
    {
      *impl().vector() += *y.impl().vector();
      return *this;
    }

//    Vector& Vector::axpy(double a, const Vector& y)
//    {
//      impl().vector()->axpy(a,*castTo<Vector>(y).impl().vector());
//      return *this;
//    }

    Vector& Vector::operator-=(const Vector& y)
    {
      *impl().vector() -= *y.impl().vector();
      return *this;
    }

    Vector& Vector::operator*=(double a)
    {
      *impl().vector() *= a;
      return *this;
    }

    Vector Vector::operator- () const
    {
      auto v = *this;
      v *= -1;
      return v;
    }

    bool Vector::operator==(const Vector& y) const
    {
      auto dx = impl().vector()->copy();
     *dx -= *y.impl().vector();
      return dx->inner( *dx ) < eps();
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

    double Vector::operator()(const Vector& y) const
    {
      return impl().vector()->inner( *y.impl().vector() );
    }
  }
}
