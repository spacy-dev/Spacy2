#include "c2Operator.hh"

#include "functionSpaceElement.hh"
#include "Interface/Operator/abstractC2Operator.hh"

namespace Algorithm
{
  C2Operator::C2Operator(std::shared_ptr<AbstractC2Operator> impl)
    : C1Operator(impl),
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

//  LinearOperator C1Operator::getLinearization(const FunctionSpaceElement& x) const
//  {
//    return LinearOperator( impl().getLinearization(x.impl()) );
//  }

  FunctionSpaceElement C2Operator::d2(const FunctionSpaceElement& x, const FunctionSpaceElement& dx, const FunctionSpaceElement& dy) const
  {
    return impl().d2(x.impl(),dx.impl(),dy.impl());
  }

  AbstractC2Operator& C2Operator::impl()
  {
    return *impl_;
  }

  const AbstractC2Operator& C2Operator::impl() const
  {
    return *impl_;
  }
}
