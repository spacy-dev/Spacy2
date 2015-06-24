#include "c1Operator.hh"

#include "functionSpaceElement.hh"
#include "Interface/Operator/abstractC1Operator.hh"

namespace Algorithm
{
  C1Operator::C1Operator(std::shared_ptr<AbstractC1Operator> impl)
    : Operator(impl),
      impl_(impl)
  {}

//  void C1Operator::setArgument(const FunctionSpaceElement &x)
//  {
//    impl().setArgument(x.impl());
//  }

//  FunctionSpaceElement C1Operator::getArgument() const
//  {
//    return FunctionSpaceElement( clone( impl().getArgument())  );
//  }

  FunctionSpaceElement C1Operator::operator ()(const FunctionSpaceElement& x) const
  {
    return impl()(x.impl());
  }

  LinearOperator C1Operator::getLinearization(const FunctionSpaceElement& x) const
  {
    return LinearOperator( impl().getLinearization(x.impl()) );
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
