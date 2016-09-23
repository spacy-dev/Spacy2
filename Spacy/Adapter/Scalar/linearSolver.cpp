#include "linearSolver.hh"

#include <Spacy/vector.hh>
#include <Spacy/Spaces/RealSpace/real.hh>
#include <Spacy/Util/cast.hh>

namespace Spacy
{
  namespace Scalar
  {
    LinearSolver::LinearSolver(double value)
      : value_(value)
    {}

    ::Spacy::Vector LinearSolver::operator()(const ::Spacy::Vector& y) const
    {
      return cast_ref<Real>(y) / value_;
    }
  }
}
