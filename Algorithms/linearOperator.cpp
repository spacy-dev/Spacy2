#include "linearOperator.hh"

#include "Interface/Operator/linearizedOperator.hh"
#include "Util/Exceptions/invalidArgumentException.hh"

namespace Algorithm
{
  LinearOperator::LinearOperator(const LinearOperator& B)
    : Operator(*this), impl_(clone(B.impl()))
  {}

  LinearOperator::LinearOperator(const LinearizedOperator& impl)
    : Operator(nullptr), impl_(clone(impl))
  {}

  LinearOperator::LinearOperator(const AbstractLinearOperator& impl)
    : Operator(nullptr), impl_(clone(impl))
  {}

  FunctionSpaceElement LinearOperator::operator()(const FunctionSpaceElement& x) const
  {
    return impl()(x.impl());
  }

  LinearSolver LinearOperator::getSolver() const
  {
    return LinearSolver( impl().getSolver() );
  }

  AbstractLinearOperator& LinearOperator::impl()
  {
    return *impl_;
  }

  const AbstractLinearOperator& LinearOperator::impl() const
  {
    return *impl_;
  }

  LinearSolver operator^(const LinearOperator& A, int k)
  {
    if( k == -1 ) return A.getSolver();
    throw InvalidArgumentException("operator^ for LinearOperator only defined for exponent -1.");
  }
}
