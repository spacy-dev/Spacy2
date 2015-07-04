#ifndef ALGORITHM_OPTIMAL_CONTROL_LAGRANGIAN_HH
#define ALGORITHM_OPTIMAL_CONTROL_LAGRANGIAN_HH

#include <memory>

#include "c2Functional.hh"
#include "c2Operator.hh"
#include "functionSpaceElement.hh"

namespace Algorithm
{
  class AbstractFunctionSpaceElement;
  class FunctionSpace;
  class ProductSpaceElement;

  class OptimalControlLagrangian
  {
  public:
    OptimalControlLagrangian(const C2Functional& costFunctional, const C2Operator& state, const C2Operator& control);

    double operator()(const FunctionSpaceElement& x);

    double d1(const FunctionSpaceElement& x, const FunctionSpaceElement& dx) const;

    double d2(const FunctionSpaceElement& x, const FunctionSpaceElement& dx, const FunctionSpaceElement& dy) const;

    const C2Functional& getCostFunctional() const;

    const C2Operator& getStateOperator() const;

    const C2Operator& getControlOperator() const;

  private:
    C2Functional f_;
    C2Operator state_, control_;
  };
}

#endif // ALGORITHM_OPTIMAL_CONTROL_LAGRANGIAN_HH
