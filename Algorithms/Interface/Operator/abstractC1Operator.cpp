#include "abstractC1Operator.hh"

#include "Interface/abstractFunctionSpaceElement.hh"
#include "linearizedOperator.hh"

namespace Algorithm
{
  AbstractC1Operator::AbstractC1Operator(std::shared_ptr<AbstractLinearSolver> solver, const AbstractBanachSpace &domain, const AbstractBanachSpace &range)
    : AbstractOperator(domain,range), solver_(solver)
  {}

  AbstractC1Operator::AbstractC1Operator(const AbstractBanachSpace &domain, const AbstractBanachSpace &range)
    : AbstractOperator(domain,range)
  {}

  LinearizedOperator AbstractC1Operator::getLinearization(const AbstractFunctionSpaceElement& x) const
  {
    return makeLinearization(x);
  }

  LinearizedOperator AbstractC1Operator::makeLinearization(const AbstractFunctionSpaceElement& x) const
  {
    return LinearizedOperator(clone(*this),x,solver_);
  }
}
