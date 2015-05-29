#ifndef ALGORITHM_INTERFACE_ABSTRACT_TWICE_DIFFERENTIABLE_FUNCTIONAL_HH
#define ALGORITHM_INTERFACE_ABSTRACT_TWICE_DIFFERENTIABLE_FUNCTIONAL_HH

#include "abstractDifferentiableFunctional.hh"

namespace Algorithm
{
  class AbstractTwiceDifferentiableFunctional : public AbstractDifferentiableFunctional
  {
  public:
    virtual double d2(const AbstractFunctionSpaceElement& dx, const AbstractFunctionSpaceElement& dy) const = 0;
  };
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_TWICE_DIFFERENTIABLE_FUNCTIONAL_HH
