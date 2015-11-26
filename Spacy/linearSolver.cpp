#include "linearSolver.hh"

namespace Spacy
{
  Vector IndefiniteLinearSolver::operator()(const Vector& x) const
  {
    return base_.impl()(x);
  }

  bool IndefiniteLinearSolver::isPositiveDefinite() const
  {
    return base_.impl().isPositiveDefinite();
  }

  IndefiniteLinearSolver::operator bool() const
  {
    return !is_empty(base_);
  }

  bool is_empty(const IndefiniteLinearSolver& solver)
  {
    return is_empty(solver.base_);
  }
}
