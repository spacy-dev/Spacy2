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
    
    double operator()(const AbstractFunctionSpaceElement& x) const;

    const AbstractBanachSpace& getDomain() const;

  protected:
    virtual double d0(const AbstractFunctionSpaceElement&) const = 0;

  private:
    const AbstractBanachSpace& domain_;
  };
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_C0_FUNCTIONAL_HH
