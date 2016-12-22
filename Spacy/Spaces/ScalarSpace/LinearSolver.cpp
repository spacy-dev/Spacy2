#include "LinearSolver.hh"

#include <Spacy/vector.hh>
#include <Spacy/Spaces/ScalarSpace/Real.hh>
#include <Spacy/Util/cast.hh>

namespace Spacy
{
    namespace Scalar
    {
        LinearSolver::LinearSolver(Real y)
            : y_(y)
        {}

        ::Spacy::Vector LinearSolver::operator()(const ::Spacy::Vector& x) const
        {
            return cast_ref<Real>(x) / y_;
        }
    }
}
