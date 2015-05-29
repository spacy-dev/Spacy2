#include "functional.hh"

#include "Interface/abstractFunctional.hh"
#include "functionSpaceElement.hh"

namespace Algorithm
{
  Functional::Functional(std::shared_ptr<AbstractFunctional> impl)
    : impl_(impl)
  {}

//  void Operator::setArgument(const FunctionSpaceElement &x)
//  {
//    impl_->setArgument(x.impl());
//  }

//  FunctionSpaceElement Operator::operator()() const
//  {
//    return (*impl_)();
//  }

  double Functional::operator()(const FunctionSpaceElement& x) const
  {
//    setArgument(x);
    return (*impl_)(x.impl());
  }
}
