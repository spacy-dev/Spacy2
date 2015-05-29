#ifndef ALGORITHM_INTERFACE_ABSTRACT_FUNCTIONAL_HH
#define ALGORITHM_INTERFACE_ABSTRACT_FUNCTIONAL_HH

#include <memory>

#include "../functionSpaceElement.hh"

namespace Algorithm
{
  class AbstractBanachSpace;

  class AbstractFunctional
  {
  public:
    virtual ~AbstractFunctional(){}
    
    virtual std::unique_ptr<AbstractFunctional> clone() const = 0;

    virtual double operator()(const AbstractFunctionSpaceElement&) const = 0;

    virtual const AbstractBanachSpace& getDomain() const = 0;
  };
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_FUNCTIONAL_HH
