#ifndef ALGORITHM_INTERFACE_ABSTRACT_C2_FUNCTIONAL_HH
#define ALGORITHM_INTERFACE_ABSTRACT_C2_FUNCTIONAL_HH

#include "abstractC1Functional.hh"
#include "Interface/Operator/abstractC1Operator.hh"
#include "Interface/Operator/linearizedOperator.hh"

namespace Algorithm
{
  class AbstractLinearOperator;

  class AbstractC2Functional : public AbstractC1Functional
  {
  public:
    AbstractC2Functional(const AbstractBanachSpace& domain);

    double d1(const AbstractFunctionSpaceElement &dx) const override;

    virtual double d2(const AbstractFunctionSpaceElement& dx, const AbstractFunctionSpaceElement& dy) const = 0;

    virtual std::unique_ptr<AbstractFunctionSpaceElement> d2(const AbstractFunctionSpaceElement &dx) const = 0;

    std::shared_ptr<AbstractC1Operator> getDerivative() const;

    void setDerivative(std::shared_ptr<AbstractC1Operator> derivative);

    using AbstractC1Functional::d1;
  private:
    std::shared_ptr<AbstractC1Operator> derivative_;
  };
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_C2_FUNCTIONAL_HH
