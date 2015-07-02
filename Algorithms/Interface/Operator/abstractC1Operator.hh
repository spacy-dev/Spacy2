#ifndef ALGORITHM_INTERFACE_ABSTRACT_C1_OPERATOR_HH
#define ALGORITHM_INTERFACE_ABSTRACT_C1_OPERATOR_HH

#include <memory>

#include "abstractOperator.hh"
//#include "linearizedOperator.hh"

namespace Algorithm
{
  class AbstractLinearizedOperator;
  class AbstractLinearSolver;
  class LinearizedOperator;

  class AbstractC1Operator : public AbstractOperator
  {
  public:
    AbstractC1Operator(const AbstractBanachSpace& domain, const AbstractBanachSpace& range);

    AbstractC1Operator(std::shared_ptr<AbstractLinearSolver> solver, const AbstractBanachSpace& domain, const AbstractBanachSpace& range);

    LinearizedOperator getLinearization(const AbstractFunctionSpaceElement& x) const;

  protected:
    friend class LinearizedOperator;

    virtual std::unique_ptr<AbstractFunctionSpaceElement> d1(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& dx) const = 0;

    virtual LinearizedOperator makeLinearization(const AbstractFunctionSpaceElement& x) const;

    mutable std::shared_ptr<AbstractLinearSolver> solver_;
  };
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_C1_OPERATOR_HH
