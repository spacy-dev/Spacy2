#include "linearSolver.hh"

#include "Spacy/vectorSpace.hh"

namespace Spacy
{
  Vector IndefiniteLinearSolver::operator()(const Vector& x) const
  {
    return base_->operator()(x);
  }

  bool IndefiniteLinearSolver::isPositiveDefinite() const
  {
    return base_->isPositiveDefinite();
  }

  IndefiniteLinearSolver::operator bool() const
  {
    return base_;
  }
}
