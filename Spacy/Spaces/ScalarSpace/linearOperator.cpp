#include "linearOperator.hh"

#include <Spacy/linearSolver.hh>
#include <Spacy/vector.hh>
#include <Spacy/Spaces/realSpace.hh>
#include <Spacy/Util/cast.hh>
#include "linearSolver.hh"

namespace Spacy
{
    namespace Scalar
    {
        LinearOperator::LinearOperator(const VectorSpace& space, Real value) :
            VectorBase(space),
            OperatorBase(Space::R,Space::R),
            Mixin::Get<Real>(value)
        {}

        ::Spacy::Vector LinearOperator::operator()(const ::Spacy::Vector& dx) const
        {
            return Real( get()*Mixin::get(cast_ref<Real>(dx)) );
        }

        ::Spacy::Real LinearOperator::operator()(const LinearOperator& dx) const
        {
            return Real( get()*dx.get() );
        }

        ::Spacy::LinearSolver LinearOperator::solver() const
        {
            return LinearSolver(get());
        }

        LinearOperator LinearOperator::operator-() const
        {
            return LinearOperator(space(),-get());
        }
    }
}
