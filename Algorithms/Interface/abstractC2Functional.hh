#ifndef ALGORITHM_INTERFACE_ABSTRACT_TWICE_C2_FUNCTIONAL_HH
#define ALGORITHM_INTERFACE_ABSTRACT_TWICE_C2_FUNCTIONAL_HH

#include "abstractC1Functional.hh"

namespace Algorithm
{
  class AbstractLinearSolver;

  class AbstractC2Functional : public AbstractC1Functional
  {
  public:
    AbstractC2Functional(const AbstractBanachSpace& domain);

    virtual double d2(const AbstractFunctionSpaceElement& dx, const AbstractFunctionSpaceElement& dy) const = 0;

    virtual std::unique_ptr<AbstractLinearSolver> inverse() const;
  };
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_TWICE_C2_FUNCTIONAL_HH
