#include "c1Operator.hh"

#include "functionSpaceElement.hh"
#include "Interface/Operator/abstractC1Operator.hh"

namespace Algorithm
{
  C1Operator::C1Operator(std::shared_ptr<AbstractC1Operator> impl)
    : Operator(impl),
      impl_(impl)
  {}

  void C1Operator::setArgument(const FunctionSpaceElement &x)
  {
    impl().setArgument(x.impl());
  }

  FunctionSpaceElement C1Operator::getArgument() const
  {
    return FunctionSpaceElement( clone( impl().getArgument())  );
  }

  FunctionSpaceElement C1Operator::operator ()() const
  {
    return impl().d0();
  }

  FunctionSpaceElement C1Operator::d1(const FunctionSpaceElement &dx) const
  {
    return FunctionSpaceElement( impl().d1(dx.impl()) );
  }

  LinearOperator C1Operator::getLinearization() const
  {
    return LinearOperator( impl().getLinearization() );
  }

  AbstractC1Operator& C1Operator::impl()
  {
    return *impl_;
  }

  const AbstractC1Operator& C1Operator::impl() const
  {
    return *impl_;
  }
}
