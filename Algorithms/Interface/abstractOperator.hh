#ifndef ALGORITHM_INTERFACE_ABSTRACT_OPERATOR_HH
#define ALGORITHM_INTERFACE_ABSTRACT_OPERATOR_HH

#include "../functionSpaceElement.hh"

namespace Algorithm
{
  class FunctionSpace;

  class AbstractOperator
  {
  public:
    virtual ~AbstractOperator(){}
    
    virtual void setArgument(const FunctionSpaceElement&) = 0;

    virtual FunctionSpaceElement operator()() const = 0;

    virtual const FunctionSpace& getDomain() const = 0;

    virtual const FunctionSpace& getRange() const = 0;
  };
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_OPERATOR_HH
