#ifndef ALGORITHM_INTERFACE_LINEARIZED_OPERATOR_HH
#define ALGORITHM_INTERFACE_LINEARIZED_OPERATOR_HH

#include <memory>

#include "abstractC1Operator.hh"
#include "abstractLinearOperator.hh"

namespace Algorithm
{
  class AbstractFunctionSpaceElement;

  class LinearizedOperator : public AbstractLinearOperator
  {
  public:
    LinearizedOperator(std::unique_ptr<AbstractC1Operator>&& A, const AbstractFunctionSpaceElement& x);

    LinearizedOperator(std::unique_ptr<AbstractC1Operator>&& A, const AbstractFunctionSpaceElement& x, std::shared_ptr<AbstractLinearSolver> solver);

    std::unique_ptr<AbstractFunctionSpaceElement> operator ()(const AbstractFunctionSpaceElement& dx) const final override;

    std::shared_ptr<AbstractLinearSolver> getSolver() const final override;

    void setSolver(std::shared_ptr<AbstractLinearSolver> solver);

  private:
    LinearizedOperator* cloneImpl() const;

    std::unique_ptr<AbstractC1Operator> A_;
    const AbstractFunctionSpaceElement& x_;
    std::shared_ptr<AbstractLinearSolver> solver_ = nullptr;
  };
}

#endif // ALGORITHM_INTERFACE_LINEARIZED_OPERATOR_HH
