#ifndef ALGORITHM_INTERFACE_ABSTRACT_C2_OPERATOR_HH
#define ALGORITHM_INTERFACE_ABSTRACT_C2_OPERATOR_HH

#include <memory>

#include "abstractC1Operator.hh"

namespace Algorithm
{
  namespace Interface
  {
    class AbstractLinearizedOperator;

    class AbstractC2Operator : public AbstractC1Operator
    {
    public:
      AbstractC2Operator(std::shared_ptr<AbstractBanachSpace> domain, std::shared_ptr<AbstractBanachSpace> range);

      AbstractC2Operator(std::shared_ptr<AbstractLinearSolver> solver, std::shared_ptr<AbstractBanachSpace> domain, std::shared_ptr<AbstractBanachSpace> range);

      virtual std::unique_ptr<AbstractFunctionSpaceElement> d2(const AbstractFunctionSpaceElement& x,
                                                               const AbstractFunctionSpaceElement& dx,
                                                               const AbstractFunctionSpaceElement& dy) const = 0;

    protected:
      using AbstractC1Operator::solver_;
//      std::shared_ptr<AbstractLinearSolver> solver_;
    };
  }
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_C2_OPERATOR_HH
