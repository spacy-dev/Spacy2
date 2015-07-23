#ifndef ALGORITHM_INTERFACE_ABSTRACT_C1_OPERATOR_HH
#define ALGORITHM_INTERFACE_ABSTRACT_C1_OPERATOR_HH

#include <memory>

#include "abstractOperator.hh"
//#include "linearizedOperator.hh"

namespace Algorithm
{
  namespace Interface
  {
    class AbstractLinearizedOperator;
    class AbstractLinearSolver;
    class LinearizedOperator;

    class AbstractC1Operator : public AbstractOperator
    {
    public:
      AbstractC1Operator(std::shared_ptr<AbstractBanachSpace> domain, std::shared_ptr<AbstractBanachSpace> range);

      AbstractC1Operator(std::shared_ptr<AbstractLinearSolver> solver, std::shared_ptr<AbstractBanachSpace> domain, std::shared_ptr<AbstractBanachSpace> range);

      LinearizedOperator getLinearization(const AbstractFunctionSpaceElement& x) const;

      virtual std::unique_ptr<AbstractFunctionSpaceElement> d1(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& dx) const = 0;

    protected:
      friend class LinearizedOperator;

      virtual LinearizedOperator makeLinearization(const AbstractFunctionSpaceElement& x) const;

      mutable std::shared_ptr<AbstractLinearSolver> solver_;
    };
  }
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_C1_OPERATOR_HH
