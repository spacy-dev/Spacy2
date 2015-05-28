#ifndef ALGORITHM_OPERATOR_HH
#define ALGORITHM_OPERATOR_HH

#include <functional>
#include <memory>

#include "functionSpace.hh"
#include "functionSpaceElement.hh"

namespace Algorithm
{
  class AbstractOperator;
  class AbstractDifferentiableOperator;
  class AbstractFunctionSpaceElement;
  class FunctionSpaceElement;

  class Operator
  {
  public:
    Operator(std::shared_ptr<AbstractOperator> impl);

//    void setArgument(const FunctionSpaceElement& x);

    FunctionSpaceElement operator()(const FunctionSpaceElement& x) const;

//    FunctionSpaceElement operator()() const;

    const FunctionSpace& getRange() const;

    const FunctionSpace& getDomain() const;

  protected:
    std::shared_ptr<AbstractOperator> impl_ = nullptr;
  };
}
#endif // ALGORITHM_OPERATOR_HH
