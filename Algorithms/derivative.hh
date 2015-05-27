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
    Derivative(const DifferentiableOperator& A);

    void setArgument(const FunctionSpaceElement& dx) override;

    FunctionSpaceElement operator()() const override;

    const FunctionSpace& getDomain() const override;

    const FunctionSpace& getRange() const override;

  private:
    const DifferentiableOperator& A_;
    FunctionSpaceElement dx_;
  };

  Operator derivative(const DifferentiableOperator& A);
}

#endif // DERIVATIVE_HH
