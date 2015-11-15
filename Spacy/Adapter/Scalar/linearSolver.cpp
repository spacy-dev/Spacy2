#include "linearSolver.hh"

#include "Spacy/vector.hh"
#include "Spacy/Spaces/RealSpace/real.hh"

namespace Spacy
{
  namespace Scalar
  {
    LinearSolver::LinearSolver(double value)
      : value_(value)
    {}

    ::Spacy::Vector LinearSolver::operator()(const ::Spacy::Vector& y) const
    {
      return Real( toDouble(y) / value_ );
    }
  }
}
