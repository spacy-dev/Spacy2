#ifndef ALGORITHM_C1_OPERATOR_HH
#define ALGORITHM_C1_OPERATOR_HH

#include <utility>

#include "c0Operator.hh"

namespace Algorithm
{
  class AbstractC1Operator;
  class FunctionSpace;
  class FunctionSpaceElement;

  class C1Operator : public C0Operator
  {
  public:
    C1Operator(std::shared_ptr<AbstractC1Operator> impl);

//    FunctionSpaceElement d1(const FunctionSpaceElement& x, const FunctionSpaceElement& dx);

    FunctionSpaceElement d1(const FunctionSpaceElement& dx) const;

    AbstractC1Operator& impl();

    const AbstractC1Operator& impl() const;

  private:
    std::shared_ptr<AbstractC1Operator> impl_;
  };

  template <class Implementation, class... Args>
  C1Operator makeC1Operator(Args&&... args)
  {
    return C1Operator( std::make_shared<Implementation>(std::forward<Args>(args)...) );
  }
}
#endif // ALGORITHM_C1_OPERATOR_HH
