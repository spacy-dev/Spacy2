#include "abstractC1Operator.hh"

#include "Interface/abstractFunctionSpaceElement.hh"

namespace Algorithm
{
  AbstractC1Operator::AbstractC1Operator(std::shared_ptr<AbstractLinearSolver> solver, const AbstractBanachSpace &domain, const AbstractBanachSpace &range)
    : AbstractOperator(domain,range), linearization_(*this,solver)
  {}

  AbstractC1Operator::AbstractC1Operator(const AbstractBanachSpace &domain, const AbstractBanachSpace &range)
    : AbstractOperator(domain,range), linearization_(*this)
  {}

  std::unique_ptr<AbstractFunctionSpaceElement> AbstractC1Operator::d0() const
  {
    return (*this)(getArgument());
  }

  void AbstractC1Operator::setArgument(const AbstractFunctionSpaceElement &x)
  {
    x_ = Algorithm::clone(x);
  }

  const AbstractFunctionSpaceElement& AbstractC1Operator::getArgument() const
  {
    return *x_;
  }

//  std::unique_ptr<AbstractFunctionSpaceElement> AbstractC1Operator::d1(const AbstractFunctionSpaceElement& dx) const
//  {
//    return (*d1_)(dx);
//  }

  LinearizedOperator& AbstractC1Operator::getLinearization()
  {
    return linearization_;
  }

  const LinearizedOperator& AbstractC1Operator::getLinearization() const
  {
    return linearization_;
  }
}
