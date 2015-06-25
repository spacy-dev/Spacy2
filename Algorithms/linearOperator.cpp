#include "linearOperator.hh"

#include "Interface/Operator/linearizedOperator.hh"

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


//  LinearizedOperator::LinearizedOperator(std::shared_ptr<AbstractLinearizedOperator> impl)
//    : impl_(impl)
//  {}

//  void LinearizedOperator::setArgument(const FunctionSpaceElement &x)
//  {
//    impl().setArgument( x.impl() );
//  }

//  FunctionSpaceElement LinearizedOperator::operator()(const FunctionSpaceElement& x) const
//  {
//    return impl()(x);
//  }

//  LinearSolver LinearizedOperator::getSolver() const
//  {
//    return LinearSolver( impl().getSolver() );
//  }

//  AbstractLinearizedOperator& LinearizedOperator::impl()
//  {
//    return *impl_;
//  }

//  const AbstractLinearizedOperator& LinearizedOperator::impl() const
//  {
//    return *impl_;
//  }
}
