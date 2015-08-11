#include "linearOperator.hh"

#include "Interface/abstractLinearSolver.hh"
#include "Interface/Operator/linearizedOperator.hh"
#include "Util/Exceptions/invalidArgumentException.hh"
#include "Util/castTo.hh"

namespace Algorithm
{
  LinearOperator::LinearOperator(std::unique_ptr<Interface::AbstractLinearOperator>&& impl)
    : Operator(std::unique_ptr<Interface::AbstractOperator>( impl.release() ))
  {}

  LinearSolver LinearOperator::solver() const
  {
    return LinearSolver( castTo<Interface::AbstractLinearOperator>(impl()).solver() );
  }

  LinearSolver LinearOperator::adjointSolver() const
  {
    return LinearSolver( castTo<Interface::AbstractLinearOperator>(impl()).adjointSolver() );
  }

  LinearSolver operator^(const LinearOperator& A, int k)
  {
    if( k == -1 ) return A.solver();
    throw InvalidArgumentException("operator^ for LinearOperator only defined for exponent: k = -1.");
  }
}
