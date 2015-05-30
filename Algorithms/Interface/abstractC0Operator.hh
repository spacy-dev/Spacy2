#ifndef ALGORITHM_INTERFACE_ABSTRACT_C0_OPERATOR_HH
#define ALGORITHM_INTERFACE_ABSTRACT_C0_OPERATOR_HH

#include <memory>

namespace Algorithm
{
  class AbstractBanachSpace;
  class AbstractFunctionSpaceElement;

  class AbstractC0Operator
  {
  public:
    virtual ~AbstractC0Operator(){}
    
    virtual std::unique_ptr<AbstractC0Operator> clone() const = 0;

    virtual std::unique_ptr<AbstractFunctionSpaceElement> operator()(const AbstractFunctionSpaceElement&) const = 0;

    virtual const AbstractBanachSpace& getDomain() const = 0;

    virtual const AbstractBanachSpace& getRange() const = 0;
  };
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_C0_OPERATOR_HH
