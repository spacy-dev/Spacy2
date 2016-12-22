#include "vector.hh"

#include <Spacy/vectorSpace.hh>
#include <Spacy/zeroVectorCreator.hh>
#include <Spacy/Spaces/ScalarSpace/Real.hh>
#include <Spacy/Util/cast.hh>
#include <Spacy/Util/Exceptions/callOfUndefinedFunctionException.hh>
#include <Spacy/Util/Exceptions/invalidArgumentException.hh>

#include "vectorSpace.hh"

#include <utility>

namespace Spacy
{
    const auto& get_creator(const VectorSpace& space)
    {
        auto&& c = creator<FEniCS::VectorCreator>(space);
        return c.get();
    }

    namespace FEniCS
    {
        Vector::Vector(const VectorSpace& V)
            : VectorBase(V),
              v_( get_creator(V) )
        {}

        Vector::Vector(const dolfin::Function& v, const VectorSpace& V)
            : VectorBase(V),
              v_(v)
        {}

        Vector& Vector::operator=(const dolfin::Function& v)
        {
            v_ = v;
            return *this;
        }

        //    Vector& Vector::axpy(double a, const Vector& y)
        //    {
        //      get().vector()->axpy(a,*castTo<Vector>(y).get().vector());
        //      return *this;
        //    }

        dolfin::GenericVector& Vector::get()
        {
            return *v_.vector();
        }

        const dolfin::GenericVector& Vector::get() const
        {
            return *v_.vector();
        }

        Real Vector::operator()(const Vector& y) const
        {
            return get().inner( y.get() );
        }
    }
}
