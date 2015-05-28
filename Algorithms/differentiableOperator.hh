#ifndef DIFFERENTIABLEOPERATOR_HH
#define DIFFERENTIABLEOPERATOR_HH

#include "functionSpaceElement.hh"
#include "operator.hh"

#include "Interface/abstractDifferentiableOperator.hh"

namespace Algorithm
{
  class DifferentiableOperator : public Operator
  {
  public:
    DifferentiableOperator(std::shared_ptr<AbstractDifferentiableOperator> impl);

//    DifferentiableOperator(const DifferentiableOperator&);

//    DifferentiableOperator& operator=(const DifferentiableOperator&);

    FunctionSpaceElement d1(const FunctionSpaceElement& x, const FunctionSpaceElement& dx);

    FunctionSpaceElement d1(const FunctionSpaceElement& dx) const;

    AbstractDifferentiableOperator& impl();

    const AbstractDifferentiableOperator& impl() const;

  private:
    std::shared_ptr<AbstractDifferentiableOperator> impl_;
  };
}
#endif // DIFFERENTIABLEOPERATOR_HH
