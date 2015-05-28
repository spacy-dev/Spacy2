#ifndef ALGORITHM_INTERFACE_ABSTRACT_OPERATOR_HH
#define ALGORITHM_INTERFACE_ABSTRACT_OPERATOR_HH

#include <memory>

#include "../functionSpaceElement.hh"

namespace Algorithm
{
  class FunctionSpace;

  class AbstractOperator
  {
  public:
    virtual ~AbstractOperator(){}
    
    virtual std::unique_ptr<AbstractOperator> clone() const = 0;

    virtual std::unique_ptr<AbstractFunctionSpaceElement> operator()(const AbstractFunctionSpaceElement&) const = 0;

    virtual const FunctionSpace& getDomain() const = 0;

    virtual const FunctionSpace& getRange() const = 0;
  };
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_OPERATOR_HH
