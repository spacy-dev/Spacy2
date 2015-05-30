#ifndef ALGORITHM_INTERFACE_ABSTRACT_C0_FUNCTIONAL_HH
#define ALGORITHM_INTERFACE_ABSTRACT_C0_FUNCTIONAL_HH

#include <memory>

#include "../functionSpaceElement.hh"

namespace Algorithm
{
  class AbstractBanachSpace;

  class AbstractC0Functional
  {
  public:
    virtual ~AbstractC0Functional(){}
    
    virtual std::unique_ptr<AbstractC0Functional> clone() const = 0;

//    virtual void setArgument(const AbstractFunctionSpaceElement& x) = 0;

    virtual double operator()(const AbstractFunctionSpaceElement&) const = 0;

//    virtual double operator()() const = 0;

    virtual const AbstractBanachSpace& getDomain() const = 0;
  };
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_C0_FUNCTIONAL_HH
