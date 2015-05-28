#ifndef ALGORITHM_DERIVATIVE_HH
#define ALGORITHM_DERIVATIVE_HH

#include "differentiableOperator.hh"
#include "functionSpaceElement.hh"
#include "operator.hh"

namespace Algorithm
{
  class Derivative : public AbstractOperator
  {
  public:
    Derivative(const AbstractDifferentiableOperator& A, const AbstractFunctionSpaceElement& x);

    std::unique_ptr<AbstractOperator> clone() const override;

    std::unique_ptr<AbstractFunctionSpaceElement> operator()(const AbstractFunctionSpaceElement& dx) const override;

    const FunctionSpace& getDomain() const override;

    const FunctionSpace& getRange() const override;

  private:
    std::unique_ptr<AbstractDifferentiableOperator> A_;
    const AbstractFunctionSpaceElement& x_;
  };

  Operator derivative(const DifferentiableOperator& A, const FunctionSpaceElement& x);
}

#endif // DERIVATIVE_HH
