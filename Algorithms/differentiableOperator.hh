#ifndef DIFFERENTIABLEOPERATOR_HH
#define DIFFERENTIABLEOPERATOR_HH

#include "operator.hh"

namespace Algorithm
{
  class AbstractDifferentiableOperator;
  class FunctionSpace;
  class FunctionSpaceElement;

  class DifferentiableOperator : public Operator
  {
  public:
    DifferentiableOperator(std::shared_ptr<AbstractDifferentiableOperator> impl);

//    FunctionSpaceElement d1(const FunctionSpaceElement& x, const FunctionSpaceElement& dx);

    FunctionSpaceElement d1(const FunctionSpaceElement& dx) const;

    AbstractDifferentiableOperator& impl();

    const AbstractDifferentiableOperator& impl() const;

  private:
    std::shared_ptr<AbstractDifferentiableOperator> impl_;
  };
}
#endif // DIFFERENTIABLEOPERATOR_HH
