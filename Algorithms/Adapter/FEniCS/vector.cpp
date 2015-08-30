#include "vector.hh"

#include "vectorSpace.hh"

#include "Algorithms/Util/cast.hh"
#include "Algorithms/Util/Exceptions/callOfUndefinedFunctionException.hh"
#include "Algorithms/Util/Exceptions/invalidArgumentException.hh"

#include <utility>

namespace Algorithm
{
  namespace FEniCS
  {
    Vector::Vector(const ::Algorithm::VectorSpace& space)
      : VectorBase<Vector>(space), //Impl<dolfin::Function>( f )
        v_(cast_ref<VectorCreator>(space.impl()).impl())
    {}

    Vector& Vector::operator=(const dolfin::Function& v)
    {
      v_ = v;
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

    double Vector::operator()(const Vector& y) const
    {
      return impl().inner( y.impl() );
    }
  }
}
