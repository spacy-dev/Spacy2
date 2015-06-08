#ifndef ALGORITHM_INTERFACE_ABSTRACT_C1_FUNCTIONAL_HH
#define ALGORITHM_INTERFACE_ABSTRACT_C1_FUNCTIONAL_HH

#include "abstractFunctional.hh"

namespace Algorithm
{
  class AbstractC1Operator;

  class AbstractC1Functional : public AbstractFunctional
  {
  public:
    AbstractC1Functional(const AbstractBanachSpace& domain);

    virtual double d0() const;

    virtual double d1(const AbstractFunctionSpaceElement& dx) const = 0;

    virtual std::unique_ptr<AbstractFunctionSpaceElement> d1() const = 0;

    virtual void setArgument(const AbstractFunctionSpaceElement& x);

    const AbstractFunctionSpaceElement& getArgument() const;

  private:
    std::shared_ptr<AbstractFunctionSpaceElement> x_;
  };
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_C1_FUNCTIONAL_HH
