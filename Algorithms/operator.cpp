#include "operator.hh"

#include "Interface/abstractOperator.hh"

namespace Algorithm
{
  Operator::Operator(std::shared_ptr<AbstractOperator> impl)
    : impl_(impl)
  {}

  void Operator::setArgument(const FunctionSpaceElement &x) const
  {
    impl_->setArgument(x);
  }

  FunctionSpaceElement Operator::operator()() const
  {
    return (*impl_)();
  }

  FunctionSpaceElement Operator::operator()(const FunctionSpaceElement& x) const
  {
    setArgument(x);
    return (*this)();
  }

  const FunctionSpace& Operator::getRange() const
  {
    return impl_->getRange();
  }

  const FunctionSpace& Operator::getDomain() const
  {
    return impl_->getDomain();
  }
}
