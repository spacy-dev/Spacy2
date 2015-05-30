#include "c2Operator.hh"

#include "functionSpaceElement.hh"
#include "Interface/abstractC2Operator.hh"

namespace Algorithm
{
  C2Operator::C2Operator(std::shared_ptr<AbstractC2Operator> impl)
    : C1Operator(impl),
      impl_(impl)
  {}

  FunctionSpaceElement C2Operator::d2(const FunctionSpaceElement &dx, const FunctionSpaceElement& dy) const
  {
    return FunctionSpaceElement( impl_->d2(dx.impl(),dy.impl()) );
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
