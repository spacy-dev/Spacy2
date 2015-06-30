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
    AbstractLinearOperator(const AbstractBanachSpace& domain, const AbstractBanachSpace& range);

    virtual std::shared_ptr<AbstractLinearSolver> getSolver() const = 0;
  };
}
#endif // ALGORITHM_INTERFACE_ABSTRACT_LINEAR_OPERATOR_HH