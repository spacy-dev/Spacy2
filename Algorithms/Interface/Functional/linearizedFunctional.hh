#ifndef ALGORITHM_INTERFACE_LINEARIZED_FUNCTIONAL_HH
#define ALGORITHM_INTERFACE_LINEARIZED_FUNCTIONAL_HH

#include <memory>

#include "abstractFunctional.hh"

namespace Algorithm
{
  class AbstractC2Functional;
  class AbstractFunctionSpaceElement;

  class LinearizedFunctional : public AbstractFunctional
  {
  public:
    LinearizedFunctional(const AbstractC2Functional& A, const AbstractFunctionSpaceElement& x);

  private:
    double d0(const AbstractFunctionSpaceElement& dx) const final override;

    LinearizedFunctional* cloneImpl() const;

    const AbstractC2Functional& A_;
    const AbstractFunctionSpaceElement& x_;
  };
}

#endif // ALGORITHM_INTERFACE_LINEARIZED_FUNCTIONAL_HH
