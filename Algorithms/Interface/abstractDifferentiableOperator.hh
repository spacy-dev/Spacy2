#ifndef ALGORITHM_INTERFACE_ABSTRACT_DIFFERENTIABLE_OPERATOR_HH
#define ALGORITHM_INTERFACE_ABSTRACT_DIFFERENTIABLE_OPERATOR_HH

#include "abstractOperator.hh"

namespace Algorithm
{
  class AbstractDifferentiableOperator : public AbstractOperator
  {
  public:
    virtual std::unique_ptr<AbstractFunctionSpaceElement> d1(const AbstractFunctionSpaceElement& dx) const = 0;
  };
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_DIFFERENTIABLE_OPERATOR_HH
