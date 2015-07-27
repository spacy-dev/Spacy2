#include "linearOperator.hh"

#include "Interface/Operator/linearizedOperator.hh"
#include "Util/Exceptions/invalidArgumentException.hh"

namespace Algorithm
{
  LinearOperator::LinearOperator(std::unique_ptr<Interface::AbstractLinearOperator>&& impl)
    : Operator(std::unique_ptr<Interface::AbstractOperator>( impl.release() ))
  {}

  LinearSolver LinearOperator::solver() const
  {
    return LinearSolver( dynamic_cast<const Interface::AbstractLinearOperator&>(impl()).solver() );
  }

  LinearSolver operator^(const LinearOperator& A, int k)
  {
    if( k == -1 ) return A.solver();
    throw InvalidArgumentException("operator^ for LinearOperator only defined for exponent: k = -1.");
  }
}
