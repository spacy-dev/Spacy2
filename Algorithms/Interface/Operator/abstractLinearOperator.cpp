#include "abstractLinearOperator.hh"

#include "Interface/abstractLinearSolver.hh"
#include "Util/Exceptions/callOfUndefinedFunctionException.hh"

namespace Algorithm
{
  Interface::AbstractLinearOperator::AbstractLinearOperator(std::shared_ptr<AbstractFunctionSpace> domain, std::shared_ptr<AbstractFunctionSpace> range)
    : AbstractOperator(domain,range)
  {}

  std::unique_ptr<Interface::AbstractLinearSolver> Interface::AbstractLinearOperator::solver() const
  {
    throw CallOfUndefinedFunctionException("Interface::AbstractLinearOperator::solver()");
  }
}
