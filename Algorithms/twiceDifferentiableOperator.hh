#ifndef ALGORITHM_TWICE_DIFFERENTIABLEOPERATOR_HH
#define ALGORITHM_TWICE_DIFFERENTIABLEOPERATOR_HH

#include "functionSpaceElement.hh"
#include "differentiableOperator.hh"

#include "Interface/abstractTwiceDifferentiableOperator.hh"

namespace Algorithm
{
  class TwiceDifferentiableOperator : public DifferentiableOperator
  {
  public:
    TwiceDifferentiableOperator(std::shared_ptr<AbstractTwiceDifferentiableOperator> impl);

//    FunctionSpaceElement d2(const FunctionSpaceElement& x, const FunctionSpaceElement& dx, const FunctionSpaceElement& dy);

    FunctionSpaceElement d2(const FunctionSpaceElement& dx, const FunctionSpaceElement& dy) const;

    AbstractTwiceDifferentiableOperator& impl();

    const AbstractTwiceDifferentiableOperator& impl() const;

  private:
    std::shared_ptr<AbstractTwiceDifferentiableOperator> impl_;
  };
}
#endif // ALGORITHM_TWICE_DIFFERENTIABLEOPERATOR_HH
