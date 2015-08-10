#include "abstractLinearSolver.hh"

namespace Algorithm
{
  namespace Interface
  {
    AbstractLinearSolver::AbstractLinearSolver(std::shared_ptr<AbstractFunctionSpace> domain, std::shared_ptr<AbstractFunctionSpace> range)
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
