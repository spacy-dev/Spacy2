#ifndef ALGORITHM_INTERFACE_ABSTRACT_TWICE_DIFFERENTIABLE_OPERATOR_HH
#define ALGORITHM_INTERFACE_ABSTRACT_TWICE_DIFFERENTIABLE_OPERATOR_HH

#include "abstractOperator.hh"

namespace Algorithm
{
  class AbstractDifferentiableOperator : public AbstractOperator
  {
  public:
    virtual std::unique_ptr<AbstractFunctionSpaceElement> d2(const AbstractFunctionSpaceElement& dx, const AbstractFunctionSpaceElement&) const = 0;
  };
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_TWICE_DIFFERENTIABLE_OPERATOR_HH
