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
    AbstractFunctional(std::shared_ptr<AbstractBanachSpace> domain);

    virtual ~AbstractFunctional();
    
    double operator()(const AbstractFunctionSpaceElement& x) const;

    const AbstractBanachSpace& getDomain() const;

    std::shared_ptr<AbstractBanachSpace> getSharedDomain() const;

  protected:
    virtual double d0(const AbstractFunctionSpaceElement&) const = 0;

  private:
    std::shared_ptr<AbstractBanachSpace> domain_;
  };
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_C0_FUNCTIONAL_HH
