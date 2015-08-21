#include "abstractLinearOperator.hh"

#include "Interface/abstractLinearSolver.hh"
#include "Util/Exceptions/callOfUndefinedFunctionException.hh"

#include "vectorSpace.hh"

namespace Algorithm
{
  Interface::AbstractLinearOperator::AbstractLinearOperator(VectorSpace* domain, VectorSpace* range)
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
