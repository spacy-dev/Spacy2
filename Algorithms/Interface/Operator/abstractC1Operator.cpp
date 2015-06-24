#include "abstractC1Operator.hh"

#include "Interface/abstractFunctionSpaceElement.hh"

namespace Algorithm
{
  AbstractC1Operator::AbstractC1Operator(std::shared_ptr<AbstractLinearSolver> solver, const AbstractBanachSpace &domain, const AbstractBanachSpace &range)
    : AbstractOperator(domain,range), solver_(solver)//, linearization_(*this,solver)
  {}

  AbstractC1Operator::AbstractC1Operator(const AbstractBanachSpace &domain, const AbstractBanachSpace &range)
    : AbstractOperator(domain,range)//, linearization_(*this)
  {}

//  std::unique_ptr<AbstractFunctionSpaceElement> AbstractC1Operator::d0() const
//  {
//    return (*this)(getArgument());
//  }

//  void AbstractC1Operator::setArgument(const AbstractFunctionSpaceElement &x)
//  {
//    x_ = clone(x);
//  }

//  const AbstractFunctionSpaceElement& AbstractC1Operator::getArgument() const
//  {
//    return *x_;
//  }

//  std::unique_ptr<AbstractFunctionSpaceElement> AbstractC1Operator::d1(const AbstractFunctionSpaceElement& dx) const
//  {
//    return (*d1_)(dx);
//  }

//  LinearizedOperator& AbstractC1Operator::getLinearization()
//  {
//    return linearization_;
//  }

  LinearizedOperator AbstractC1Operator::getLinearization(const AbstractFunctionSpaceElement& x) const
  {
    return makeLinearization(x);
  }

  LinearizedOperator AbstractC1Operator::makeLinearization(const AbstractFunctionSpaceElement& x) const
  {
    return LinearizedOperator(*this,x,solver_);
  }
}
