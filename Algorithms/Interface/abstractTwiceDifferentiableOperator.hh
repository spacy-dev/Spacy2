#ifndef ALGORITHM_INTERFACE_ABSTRACT_TWICE_DIFFERENTIABLE_OPERATOR_HH
#define ALGORITHM_INTERFACE_ABSTRACT_TWICE_DIFFERENTIABLE_OPERATOR_HH

#include "abstractDifferentiableOperator.hh"

namespace Algorithm
{
  class AbstractTwiceDifferentiableOperator : public AbstractDifferentiableOperator
  {
  public:
    virtual std::unique_ptr<AbstractFunctionSpaceElement> d2(const AbstractFunctionSpaceElement& dx, const AbstractFunctionSpaceElement&) const = 0;
  };
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_TWICE_DIFFERENTIABLE_OPERATOR_HH
