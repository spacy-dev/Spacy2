#include "abstractC1Operator.hh"

#include "Interface/abstractFunctionSpaceElement.hh"
#include "linearizedOperator.hh"
#include "Util/Exceptions/callOfUndefinedFunctionException.hh"

namespace Algorithm
{
  namespace Interface
  {
    AbstractC1Operator::AbstractC1Operator(std::shared_ptr<AbstractBanachSpace> domain, std::shared_ptr<AbstractBanachSpace> range)
      : AbstractOperator(domain,range)
    {}

    std::unique_ptr<LinearizedOperator> AbstractC1Operator::linearization(const AbstractFunctionSpaceElement& x) const
    {
      return makeLinearization(x);
    }

    std::unique_ptr<LinearizedOperator> AbstractC1Operator::makeLinearization(const AbstractFunctionSpaceElement& x) const
    {
      return std::make_unique<LinearizedOperator>(clone(this),x);
    }

    std::unique_ptr<AbstractLinearSolver> AbstractC1Operator::makeSolver() const
    {
      throw CallOfUndefinedFunctionException("AbstractC1Operator::makeSolver");
    }
  }
}
