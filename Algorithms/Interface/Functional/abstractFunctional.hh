#ifndef ALGORITHM_INTERFACE_ABSTRACT_FUNCTIONAL_HH
#define ALGORITHM_INTERFACE_ABSTRACT_FUNCTIONAL_HH

#include <memory>

#include "Util/cloneable.hh"

namespace Algorithm
{
  class AbstractBanachSpace;
  class AbstractFunctionSpaceElement;

  class AbstractFunctional : public Cloneable<AbstractFunctional>
  {
  public:
    AbstractFunctional(const AbstractBanachSpace& domain);

    virtual ~AbstractFunctional();
    
    virtual double operator()(const AbstractFunctionSpaceElement&) const = 0;

    const AbstractBanachSpace& getDomain() const;

  private:
    const AbstractBanachSpace& domain_;
  };
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_C0_FUNCTIONAL_HH
