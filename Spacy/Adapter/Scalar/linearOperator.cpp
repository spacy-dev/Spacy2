#include "linearOperator.hh"

#include <Spacy/linearSolver.hh>
#include <Spacy/vector.hh>
#include <Spacy/Spaces/realSpace.hh>
#include "linearSolver.hh"

namespace Spacy
{
    namespace Scalar
    {
        LinearOperator::LinearOperator(const VectorSpace& space, double value) :
            VectorBase(space),
            OperatorBase(Space::R,Space::R),
            Mixin::Get<double>(value)
        {}

        ::Spacy::Vector LinearOperator::operator()(const ::Spacy::Vector& dx) const
        {
            return Real( get()*toDouble(dx) );
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
