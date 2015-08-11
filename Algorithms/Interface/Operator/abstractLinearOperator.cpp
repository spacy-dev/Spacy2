#include "abstractLinearOperator.hh"

#include "Interface/abstractLinearSolver.hh"
#include "Util/Exceptions/callOfUndefinedFunctionException.hh"

namespace Algorithm
{
  Interface::AbstractLinearOperator::AbstractLinearOperator(std::shared_ptr<AbstractVectorSpace> domain, std::shared_ptr<AbstractVectorSpace> range)
    : AbstractOperator(domain,range)
  {}

  std::unique_ptr<Interface::AbstractLinearSolver> Interface::AbstractLinearOperator::solver() const
  {
    throw CallOfUndefinedFunctionException("Interface::AbstractLinearOperator::solver()");
  }

  std::unique_ptr<Interface::AbstractLinearSolver> Interface::AbstractLinearOperator::adjointSolver() const
  {
    throw CallOfUndefinedFunctionException("Interface::AbstractLinearOperator::adjointSolver()");
  }
}
