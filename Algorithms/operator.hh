#ifndef ALGORITHM_OPERATOR_HH
#define ALGORITHM_OPERATOR_HH

#include <functional>
#include <memory>

#include "functionSpace.hh"
#include "functionSpaceElement.hh"

namespace Algorithm
{
  class AbstractOperator;
  class FunctionSpaceElement;

  class Operator
  {
  public:
    Operator(std::shared_ptr<AbstractOperator> impl);

//    void setArgument(const FunctionSpaceElement& x);

    FunctionSpaceElement operator()(const FunctionSpaceElement& x) const;

//    FunctionSpaceElement operator()() const;

    const AbstractOperator& impl() const;

  protected:
    std::shared_ptr<AbstractOperator> impl_ = nullptr;

  };
}
#endif // ALGORITHM_OPERATOR_HH
