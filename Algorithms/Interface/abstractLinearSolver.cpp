#include "abstractLinearSolver.hh"

namespace Algorithm
{
  namespace Interface
  {
    AbstractLinearSolver::AbstractLinearSolver(std::shared_ptr<AbstractVectorSpace> domain, std::shared_ptr<AbstractVectorSpace> range)
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
