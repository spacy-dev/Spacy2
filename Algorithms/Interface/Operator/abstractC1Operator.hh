#ifndef ALGORITHM_INTERFACE_ABSTRACT_C1_OPERATOR_HH
#define ALGORITHM_INTERFACE_ABSTRACT_C1_OPERATOR_HH

#include <memory>

#include "abstractOperator.hh"
#include "linearizedOperator.hh"

namespace Algorithm
{
  class AbstractLinearizedOperator;

  class AbstractC1Operator : public AbstractOperator
  {
  public:
    AbstractC1Operator(const AbstractBanachSpace& domain, const AbstractBanachSpace& range);

    AbstractC1Operator(std::shared_ptr<AbstractLinearSolver> solver, const AbstractBanachSpace& domain, const AbstractBanachSpace& range);

    virtual void setArgument(const AbstractFunctionSpaceElement& x);

    const AbstractFunctionSpaceElement& getArgument() const;

    std::unique_ptr<AbstractFunctionSpaceElement> d0() const;

    virtual std::unique_ptr<AbstractFunctionSpaceElement> d1(const AbstractFunctionSpaceElement& dx) const = 0;

    LinearizedOperator& getLinearization();

    const LinearizedOperator& getLinearization() const;

  private:
    std::shared_ptr<AbstractFunctionSpaceElement> x_;
    LinearizedOperator linearization_;
  };
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_C1_OPERATOR_HH
