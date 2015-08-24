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
      : VectorBase<Vector>(space), //Impl<dolfin::Function>( f )
        v_(f)
    {}

    Vector& Vector::operator=(const dolfin::Function& v)
    {
      v_ = v;
      return *this;
    }

    Vector& Vector::operator=(const Vector& y)
    {
      impl() = y.impl();
//      *impl().vector() = *y.impl().vector();
      return *this;
    }

//    Vector& Vector::axpy(double a, const Vector& y)
//    {
//      impl().vector()->axpy(a,*castTo<Vector>(y).impl().vector());
//      return *this;
//    }

    unsigned Vector::size() const
    {
      return impl().size(); // todo generalize
    }

    dolfin::GenericVector& Vector::impl()
    {
      return *v_.vector();
    }

    const dolfin::GenericVector& Vector::impl() const
    {
      return *v_.vector();
    }

//    dolfin::Function& Vector::operator[](unsigned i)
//    {
//      return impl()[i];
//    }

//    const dolfin::Function& Vector::operator[](unsigned i) const
//    {
//      return impl()[i];
//    }

    double Vector::operator()(const Vector& y) const
    {
      return impl().inner( y.impl() );
    }
  }
}
