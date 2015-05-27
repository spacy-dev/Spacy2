#ifndef ALGORITHM_INTERFACE_ABSTRACT_DIFFERENTIABLE_OPERATOR_HH
#define ALGORITHM_INTERFACE_ABSTRACT_DIFFERENTIABLE_OPERATOR_HH

#include "abstractOperator.hh"

namespace Algorithm
{
  class AbstractDifferentiableOperator : public AbstractOperator
  {
  public:
    virtual FunctionSpaceElement d1(const FunctionSpaceElement& x, const FunctionSpaceElement& dx) = 0;

    virtual FunctionSpaceElement d1(const FunctionSpaceElement& dx) const = 0;
  };
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_DIFFERENTIABLE_OPERATOR_HH
