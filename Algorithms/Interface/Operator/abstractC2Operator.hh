#ifndef ALGORITHM_INTERFACE_ABSTRACT_C2_OPERATOR_HH
#define ALGORITHM_INTERFACE_ABSTRACT_C2_OPERATOR_HH

#include <memory>

#include "abstractC1Operator.hh"
#include "linearizedOperator.hh"

namespace Algorithm
{
  class AbstractLinearizedOperator;

  class AbstractC2Operator : public AbstractC1Operator
  {
  public:
    AbstractC2Operator(const AbstractBanachSpace& domain, const AbstractBanachSpace& range);

    AbstractC2Operator(std::shared_ptr<AbstractLinearSolver> solver, const AbstractBanachSpace& domain, const AbstractBanachSpace& range);

    //LinearizedOperator getLinearization(const AbstractFunctionSpaceElement& x) const;

    virtual std::unique_ptr<AbstractFunctionSpaceElement> d2(const AbstractFunctionSpaceElement& x,
                                                             const AbstractFunctionSpaceElement& dx,
                                                             const AbstractFunctionSpaceElement& dy) const = 0;

  protected:
    friend class LinearizedOperator;


//    virtual LinearizedOperator makeLinearization(const AbstractFunctionSpaceElement& x) const;

    std::shared_ptr<AbstractLinearSolver> solver_;
  };
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_C2_OPERATOR_HH
