#ifndef ALGORITHM_INTERFACE_ABSTRACT_C1_FUNCTIONAL_HH
#define ALGORITHM_INTERFACE_ABSTRACT_C1_FUNCTIONAL_HH

#include "abstractC0Functional.hh"

namespace Algorithm
{
  class AbstractC1Functional : public AbstractC0Functional
  {
  public:
    virtual double d1(const AbstractFunctionSpaceElement& dx) const = 0;
  };
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_C1_FUNCTIONAL_HH
