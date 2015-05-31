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

    void setArgument(const FunctionSpaceElement& x);

    FunctionSpaceElement operator()(const FunctionSpaceElement& x) const;

    FunctionSpaceElement operator()() const;

    AbstractC0Operator& impl();

    const AbstractC0Operator& impl() const;

  protected:
    std::shared_ptr<AbstractC0Operator> impl_ = nullptr;
  };

  template <class Implementation, class... Args>
  C0Operator makeC0Operator(Args&&... args)
  {
    return C0Operator( std::make_shared<Implementation>(std::forward<Args>(args)...) );
  }
}
#endif // ALGORITHM_C0_OPERATOR_HH
