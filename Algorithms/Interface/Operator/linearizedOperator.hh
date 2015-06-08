#ifndef ALGORITHM_INTERFACE_LINEARIZED_OPERATOR_HH
#define ALGORITHM_INTERFACE_LINEARIZED_OPERATOR_HH

#include <memory>

#include "abstractLinearOperator.hh"

namespace Algorithm
{
  class AbstractC1Operator;
  class AbstractFunctionSpaceElement;

  class LinearizedOperator : public AbstractLinearOperator
  {
  public:
    LinearizedOperator(const AbstractC1Operator& A);

    LinearizedOperator(const AbstractC1Operator& A, std::shared_ptr<AbstractLinearSolver> solver);

    std::unique_ptr<AbstractFunctionSpaceElement> operator ()(const AbstractFunctionSpaceElement& dx) const final override;

    std::shared_ptr<AbstractLinearSolver> getSolver() const final override;

    void setSolver(std::shared_ptr<AbstractLinearSolver> solver);

  private:
    LinearizedOperator* cloneImpl() const;

    const AbstractC1Operator& A_;
    std::shared_ptr<AbstractLinearSolver> solver_ = nullptr;
  };
}

#endif // ALGORITHM_INTERFACE_LINEARIZED_OPERATOR_HH
