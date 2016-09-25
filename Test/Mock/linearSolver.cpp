#include "linearSolver.hh"

#include <Spacy/vector.hh>

namespace Mock
{
  Spacy::Vector IndefiniteLinearSolver::operator ()(const Spacy::Vector& x) const
  {
    return x;
  }

  bool IndefiniteLinearSolver::isPositiveDefinite() const
  {
    return true;
  }
}
