#ifndef ALGORITHM_C1_OPERATOR_HH
#define ALGORITHM_C1_OPERATOR_HH

#include <utility>

#include "functionSpaceElement.hh"
#include "linearOperator.hh"
#include "operator.hh"

namespace Algorithm
{
  class AbstractC1Operator;
  class FunctionSpace;

  class C1Operator : public Operator
  {
  public:
    C1Operator(std::shared_ptr<AbstractC1Operator> impl);

//    void setArgument(const FunctionSpaceElement& x);

//    FunctionSpaceElement getArgument() const;

    FunctionSpaceElement operator()(const FunctionSpaceElement& x) const;

//    FunctionSpaceElement d1(const FunctionSpaceElement& x, const FunctionSpaceElement& dx);

    LinearOperator getLinearization(const FunctionSpaceElement& x) const;

    AbstractC1Operator& impl();

    const AbstractC1Operator& impl() const;

    using Operator::operator ();

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
