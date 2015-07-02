#ifndef ALGORITHM_INTERFACE_ABSTRACT_LINEAR_OPERATOR_HH
#define ALGORITHM_INTERFACE_ABSTRACT_LINEAR_OPERATOR_HH

#include "abstractOperator.hh"

#include <utility>

namespace Algorithm
{
  class AbstractBanachSpace;
  class AbstractLinearSolver;

  class AbstractLinearOperator : public AbstractOperator
  {
  public:
    AbstractLinearOperator(std::shared_ptr<AbstractBanachSpace> domain, std::shared_ptr<AbstractBanachSpace> range);

    virtual std::shared_ptr<AbstractLinearSolver> getSolver() const = 0;
  };
}
#endif // ALGORITHM_INTERFACE_ABSTRACT_LINEAR_OPERATOR_HH
