#ifndef ALGORITHM_INTERFACE_ABSTRACT_C2_OPERATOR_HH
#define ALGORITHM_INTERFACE_ABSTRACT_C2_OPERATOR_HH

#include "abstractC1Operator.hh"

namespace Algorithm
{
  class AbstractC2Operator : public AbstractC1Operator
  {
  public:
    virtual std::unique_ptr<AbstractFunctionSpaceElement> d2(const AbstractFunctionSpaceElement& dx, const AbstractFunctionSpaceElement&) const = 0;
  };
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_C2_OPERATOR_HH
