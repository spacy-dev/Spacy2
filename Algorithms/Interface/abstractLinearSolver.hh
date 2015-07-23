#ifndef ALGORITHM_INTERFACE_ABSTRACT_LINEAR_SOLVER_HH
#define ALGORITHM_INTERFACE_ABSTRACT_LINEAR_SOLVER_HH

#include <memory>

#include "Interface/Operator/abstractOperator.hh"

namespace Algorithm
{
  namespace Interface
  {
    class AbstractBanachSpace;
    class AbstractFunctionSpaceElement;

    class AbstractLinearSolver : public AbstractOperator
    {
    public:
      AbstractLinearSolver(std::shared_ptr<AbstractBanachSpace> domain, std::shared_ptr<AbstractBanachSpace> range);

      virtual ~AbstractLinearSolver();

      virtual std::unique_ptr<AbstractFunctionSpaceElement> operator()(const AbstractFunctionSpaceElement&) const = 0;

      virtual bool encounteredNonconvexity() const;
    };
  }
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_LINEAR_SOLVER_HH
