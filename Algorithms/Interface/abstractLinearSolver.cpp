#include "abstractLinearSolver.hh"

namespace Algorithm
{
  namespace Interface
  {
    AbstractLinearSolver::AbstractLinearSolver(VectorSpace* domain, VectorSpace* range)
      : AbstractOperator(domain,range)
    {}

    AbstractLinearSolver::~AbstractLinearSolver()
    {}

    bool AbstractLinearSolver::systemIsPositiveDefinite() const
    {
      return false;
    }
  }
}
