#ifndef ALGORITHM_C0_OPERATOR_HH
#define ALGORITHM_C0_OPERATOR_HH

#include <memory>

namespace Algorithm
{
  class AbstractC0Operator;
  class FunctionSpaceElement;

  class C0Operator
  {
  public:
    C0Operator(std::shared_ptr<AbstractC0Operator> impl);

//    void setArgument(const FunctionSpaceElement& x);

    FunctionSpaceElement operator()(const FunctionSpaceElement& x) const;

//    FunctionSpaceElement operator()() const;

    const AbstractC0Operator& impl() const;

  protected:
    std::shared_ptr<AbstractC0Operator> impl_ = nullptr;

  };
}
#endif // ALGORITHM_C0_OPERATOR_HH
