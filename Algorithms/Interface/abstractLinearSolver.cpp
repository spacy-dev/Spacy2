#include "abstractLinearSolver.hh"

namespace Algorithm
{
  namespace Interface
  {
    AbstractLinearSolver::AbstractLinearSolver(std::shared_ptr<AbstractBanachSpace> domain, std::shared_ptr<AbstractBanachSpace> range)
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
