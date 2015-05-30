#ifndef ALGORITHM_C1_OPERATOR_HH
#define ALGORITHM_C1_OPERATOR_HH

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
}
#endif // ALGORITHM_C1_OPERATOR_HH
