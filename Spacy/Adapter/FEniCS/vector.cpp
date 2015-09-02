#include "vector.hh"

#include "vectorSpace.hh"

#include "Spacy/Util/cast.hh"
#include "Spacy/Util/Exceptions/callOfUndefinedFunctionException.hh"
#include "Spacy/Util/Exceptions/invalidArgumentException.hh"

#include <utility>

namespace Spacy
{
  namespace FEniCS
  {
    Vector::Vector(const VectorSpace& space)
      : VectorBase<Vector>(space),
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
