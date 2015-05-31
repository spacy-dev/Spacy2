#include "inverseOperator.hh"

#include "Interface/abstractC0Operator.hh"
#include "Interface/abstractLinearSolver.hh"
#include "c0Operator.hh"
#include "functionSpaceElement.hh"

#include <utility>

namespace Algorithm
{
  InverseOperator::InverseOperator(std::unique_ptr<AbstractLinearSolver>&& impl)
    : impl_(std::move(impl))
  {}

  InverseOperator::InverseOperator(const C0Operator &A)
    : impl_(A.impl().inverse())
  {}

  InverseOperator::InverseOperator(InverseOperator&& other)
    : impl_(std::move(other.impl_))
  {}

  InverseOperator& InverseOperator::operator=(InverseOperator&& other)
  {
    impl_ = std::move(other.impl_);
    return *this;
  }

  FunctionSpaceElement InverseOperator::operator()(const FunctionSpaceElement& x, const FunctionSpaceElement& y) const
  {
    return (*impl_)(x,y);
  }

  FunctionSpaceElement InverseOperator::operator()(const FunctionSpaceElement& y) const
  {
    return (*impl_)(y);
  }
}
