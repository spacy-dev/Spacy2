#ifndef ALGORITHM_INTERFACE_ABSTRACT_OPERATOR_HH
#define ALGORITHM_INTERFACE_ABSTRACT_OPERATOR_HH

#include <memory>

#include "Util/cloneable.hh"

namespace Algorithm
{
  class AbstractBanachSpace;
  class AbstractFunctionSpaceElement;

  class AbstractOperator : public Cloneable<AbstractOperator>
  {
  public:
    using Base = AbstractOperator;

    AbstractOperator(const AbstractBanachSpace& domain, const AbstractBanachSpace& range);

    virtual ~AbstractOperator();
    
    virtual std::unique_ptr<AbstractFunctionSpaceElement> operator()(const AbstractFunctionSpaceElement&) const = 0;

    const AbstractBanachSpace& getDomain() const;

    const AbstractBanachSpace& getRange() const;

  private:
    const AbstractBanachSpace& domain_;
    const AbstractBanachSpace& range_;
  };
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_OPERATOR_HH
