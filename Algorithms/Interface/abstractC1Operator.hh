#ifndef ALGORITHM_INTERFACE_ABSTRACT_C1_OPERATOR_HH
#define ALGORITHM_INTERFACE_ABSTRACT_C1_OPERATOR_HH

#include "abstractC0Operator.hh"

namespace Algorithm
{
  class AbstractC1Operator : public AbstractC0Operator
  {
  public:
    virtual std::unique_ptr<AbstractFunctionSpaceElement> d1(const AbstractFunctionSpaceElement& dx) const = 0;
  };
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_C1_OPERATOR_HH
