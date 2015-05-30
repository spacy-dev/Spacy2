#include "c0Functional.hh"

#include "Interface/abstractC0Functional.hh"
#include "functionSpaceElement.hh"

namespace Algorithm
{
  C0Functional::C0Functional(std::shared_ptr<AbstractC0Functional> impl)
    : impl_(impl)
  {}

//  void C0Functional::setArgument(const FunctionSpaceElement &x)
//  {
//    impl_->setArgument(x.impl());
//  }

//  FunctionSpaceElement C0Functional::operator()() const
//  {
//    return (*impl_)();
//  }

  double C0Functional::operator()(const FunctionSpaceElement& x) const
  {
//    setArgument(x);
    return (*impl_)(x.impl());
  }
}
