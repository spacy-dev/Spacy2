#include "differentiableOperator.hh"

namespace Algorithm
{
  DifferentiableOperator::DifferentiableOperator(std::shared_ptr<AbstractDifferentiableOperator> impl)
    : Operator(impl),
      impl_(impl)
  {}

//  DifferentiableOperator::DifferentiableOperator(const DifferentiableOperator& A)
//    : Operator(A.impl_->clone()),
//      impl_(dynamic_cast<AbstractDifferentiableOperator*>(A.impl_->clone().release()))
//  {}

//  DifferentiableOperator& DifferentiableOperator::operator=(const DifferentiableOperator& A)
//  {
//    Operator::operator=(A);
//    impl_.reset( dynamic_cast<AbstractDifferentiableOperator*>(A.impl_->clone().release()) );
//  }

//  FunctionSpaceElement DifferentiableOperator::d1(const FunctionSpaceElement &x, const FunctionSpaceElement &dx)
//  {
//    impl_->setArgument(x.impl());
//    return FunctionSpaceElement( impl_->d1(dx.impl()) );
//  }

  FunctionSpaceElement DifferentiableOperator::d1(const FunctionSpaceElement &dx) const
  {
    return FunctionSpaceElement( impl_->d1(dx.impl()) );
  }


  AbstractDifferentiableOperator& DifferentiableOperator::impl()
  {
    return *impl_;
  }

  const AbstractDifferentiableOperator& DifferentiableOperator::impl() const
  {
    return *impl_;
  }
}
