#ifndef ALGORITHM_DERIVATIVE_HH
#define ALGORITHM_DERIVATIVE_HH

#include "c0Operator.hh"
#include "c1Operator.hh"

#include "Interface/abstractC0Operator.hh"

namespace Algorithm
{
  class AbstractC1Operator;

  class Derivative : public AbstractC0Operator
  {
  public:
    Derivative(const AbstractC1Operator& A, const AbstractFunctionSpaceElement& x);

    std::unique_ptr<AbstractC0Operator> clone() const override;

    std::unique_ptr<AbstractFunctionSpaceElement> operator()(const AbstractFunctionSpaceElement& dx) const override;

    const AbstractBanachSpace& getDomain() const override;

    const AbstractBanachSpace& getRange() const override;

  private:
    std::unique_ptr<AbstractC1Operator> A_;
    const AbstractFunctionSpaceElement& x_;
  };

  C0Operator derivative(const C1Operator& A, const FunctionSpaceElement& x);


  template <class Operator> class DerivativeMapping;

  template <>
  class DerivativeMapping<C1Operator> : public AbstractC0Operator
  {
  public:
    DerivativeMapping(const AbstractC1Operator& A, const AbstractFunctionSpaceElement& x, const AbstractBanachSpace& range);

  private:
    const AbstractC1Operator& A_;
    const AbstractFunctionSpaceElement& x_;
    const AbstractBanachSpace& range;
  };
}

#endif // DERIVATIVE_HH
