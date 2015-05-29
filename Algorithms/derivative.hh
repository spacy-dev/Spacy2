#ifndef ALGORITHM_DERIVATIVE_HH
#define ALGORITHM_DERIVATIVE_HH

#include "differentiableOperator.hh"
#include "operator.hh"

#include "Interface/abstractOperator.hh"

namespace Algorithm
{
  class AbstractDifferentiableOperator;

  class Derivative : public AbstractOperator
  {
  public:
    Derivative(const AbstractDifferentiableOperator& A, const AbstractFunctionSpaceElement& x);

    std::unique_ptr<AbstractOperator> clone() const override;

    std::unique_ptr<AbstractFunctionSpaceElement> operator()(const AbstractFunctionSpaceElement& dx) const override;

    const AbstractBanachSpace& getDomain() const override;

    const AbstractBanachSpace& getRange() const override;

  private:
    std::unique_ptr<AbstractDifferentiableOperator> A_;
    const AbstractFunctionSpaceElement& x_;
  };

  Operator derivative(const DifferentiableOperator& A, const FunctionSpaceElement& x);


  template <class Operator> class DerivativeMapping;

  template <>
  class DerivativeMapping<DifferentiableOperator> : public AbstractOperator
  {
  public:
    DerivativeMapping(const AbstractDifferentiableOperator& A, const AbstractFunctionSpaceElement& x, const AbstractBanachSpace& range);

  private:
    const AbstractDifferentiableOperator& A_;
    const AbstractFunctionSpaceElement& x_;
    const AbstractBanachSpace& range;
  };
}

#endif // DERIVATIVE_HH
