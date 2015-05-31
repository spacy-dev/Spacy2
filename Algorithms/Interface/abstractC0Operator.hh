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
    AbstractC0Operator(const AbstractBanachSpace& domain, const AbstractBanachSpace& range);

    virtual ~AbstractC0Operator();
    
    virtual std::unique_ptr<AbstractC0Operator> clone() const = 0;

    virtual std::unique_ptr<AbstractFunctionSpaceElement> operator()(const AbstractFunctionSpaceElement&) const = 0;

    const AbstractBanachSpace& getDomain() const;

    const AbstractBanachSpace& getRange() const;

  private:
    const AbstractBanachSpace& domain_;
    const AbstractBanachSpace& range_;
  };
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_C0_OPERATOR_HH
