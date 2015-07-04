#include "linearOperator.hh"

#include "Interface/Operator/linearizedOperator.hh"
#include "Util/Exceptions/invalidArgumentException.hh"

namespace Algorithm
{
  LinearOperator::LinearOperator(const Interface::AbstractLinearOperator& impl)
    : Operator(std::unique_ptr<Interface::AbstractOperator>( clone(impl).release() ))
  {}

  LinearSolver LinearOperator::getSolver() const
  {
    return LinearSolver( dynamic_cast<const Interface::AbstractLinearOperator&>(impl()).getSolver() );
  }

  LinearSolver operator^(const LinearOperator& A, int k)
  {
    if( k == -1 ) return A.getSolver();
    throw InvalidArgumentException("operator^ for LinearOperator only defined for exponent -1.");
  }
}
