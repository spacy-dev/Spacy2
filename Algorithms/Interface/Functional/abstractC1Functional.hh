#ifndef ALGORITHM_INTERFACE_ABSTRACT_C1_FUNCTIONAL_HH
#define ALGORITHM_INTERFACE_ABSTRACT_C1_FUNCTIONAL_HH

#include "abstractFunctional.hh"
#include "Interface/Operator/linearizedOperator.hh"

namespace Algorithm
{
  class AbstractC1Operator;

  class AbstractC1Functional : public AbstractFunctional
  {
  public:
    AbstractC1Functional(const AbstractBanachSpace& domain);


//    virtual double d0() const;

//    virtual double d1(const AbstractFunctionSpaceElement& dx) const = 0;

//    LinearizedOperator getLinearization(const AbstractFunctionSpaceElement& x) const;

    virtual double d1(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& dx) const = 0;

//    virtual LinearizedOperator makeLinearization(const AbstractFunctionSpaceElement& x) const;

//    virtual void setArgument(const AbstractFunctionSpaceElement& x);

//    const AbstractFunctionSpaceElement& getArgument() const;

//  private:
//    std::shared_ptr<AbstractFunctionSpaceElement> x_;
  };
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_C1_FUNCTIONAL_HH
