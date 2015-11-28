#include "linearOperator.hh"

#include "Spacy/linearSolver.hh"
#include "Spacy/vector.hh"
#include "Spacy/Spaces/realSpace.hh"
#include "linearSolver.hh"

namespace Spacy
{
  namespace Scalar
  {
    LinearOperator::LinearOperator(const VectorSpace& space, double value) :
      VectorBase(space),
      OperatorBase(Space::R,Space::R),
      Mixin::Impl<double>(value)
    {}

    ::Spacy::Vector LinearOperator::operator()(const ::Spacy::Vector& dx) const
    {
      return Real( impl()*toDouble(dx) );
    }

    ::Spacy::LinearSolver LinearOperator::solver() const
    {
      return LinearSolver(impl());
    }

    LinearOperator LinearOperator::operator-() const
    {
      return LinearOperator(*space(),-impl());
    }
  }
}
