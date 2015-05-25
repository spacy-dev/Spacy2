#ifndef ALGORITHM_INVERSE_OPERATOR_HH
#define ALGORITHM_INVERSE_OPERATOR_HH

#include <memory>

namespace Algorithm
{
  class FunctionSpaceElement;
  class AbstractLinearSolver;

  class InverseOperator
  {
  public:

    InverseOperator(std::unique_ptr<AbstractLinearSolver>&& impl);

    FunctionSpaceElement operator()(const FunctionSpaceElement& x, const FunctionSpaceElement& y) const;

    FunctionSpaceElement operator()(const FunctionSpaceElement& y) const;

  private:
    std::shared_ptr<AbstractLinearSolver> impl_;
  };
}

#endif // ALGORITHM_INVERSE_OPERATOR_HH
