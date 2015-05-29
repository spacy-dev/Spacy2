#ifndef ALGORITHM_INTERFACE_ABSTRACT_OPERATOR_HH
#define ALGORITHM_INTERFACE_ABSTRACT_OPERATOR_HH

#include <memory>

namespace Algorithm
{
  class AbstractBanachSpace;
  class AbstractFunctionSpaceElement;

  class AbstractOperator
  {
  public:
    virtual ~AbstractOperator(){}
    
    virtual std::unique_ptr<AbstractOperator> clone() const = 0;

    virtual std::unique_ptr<AbstractFunctionSpaceElement> operator()(const AbstractFunctionSpaceElement&) const = 0;

    virtual const AbstractBanachSpace& getDomain() const = 0;

    virtual const AbstractBanachSpace& getRange() const = 0;
  };
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_OPERATOR_HH
