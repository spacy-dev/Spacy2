#include "abstractC2Operator.hh"

#include "Interface/abstractFunctionSpaceElement.hh"

namespace Algorithm
{
  AbstractC2Operator::AbstractC2Operator(std::shared_ptr<AbstractLinearSolver> solver, const AbstractBanachSpace &domain, const AbstractBanachSpace &range)
    : AbstractC1Operator(domain,range), solver_(solver)//, linearization_(*this,solver)
  {}

  AbstractC2Operator::AbstractC2Operator(const AbstractBanachSpace &domain, const AbstractBanachSpace &range)
    : AbstractC1Operator(domain,range)//, linearization_(*this)
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

//  LinearizedOperator AbstractC2Operator::getLinearization(const AbstractFunctionSpaceElement& x) const
//  {
//    return makeLinearization(x);
//  }

//  LinearizedOperator AbstractC2Operator::makeLinearization(const AbstractFunctionSpaceElement& x) const
//  {
//    return LinearizedOperator(*this,x,solver_);
//  }
}
