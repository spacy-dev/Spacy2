#ifndef ALGORITHM_INTERFACE_ABSTRACT_DIFFERENTIABLE_FUNCTIONAL_HH
#define ALGORITHM_INTERFACE_ABSTRACT_DIFFERENTIABLE_FUNCTIONAL_HH

#include "abstractFunctional.hh"

namespace Algorithm
{
  class AbstractDifferentiableFunctional : public AbstractFunctional
  {
  public:
    virtual double d1(const AbstractFunctionSpaceElement& dx) const = 0;
  };
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_DIFFERENTIABLE_FUNCTIONAL_HH
