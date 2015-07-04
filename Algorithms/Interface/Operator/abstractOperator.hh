#ifndef ALGORITHM_INTERFACE_ABSTRACT_OPERATOR_HH
#define ALGORITHM_INTERFACE_ABSTRACT_OPERATOR_HH

#include <memory>

#include "Util/Mixins/cloneable.hh"

namespace Algorithm
{
  namespace Interface
  {
    class AbstractBanachSpace;
    class AbstractFunctionSpaceElement;

    class AbstractOperator : public Mixin::Cloneable<AbstractOperator>
    {
    public:
      using Base = AbstractOperator;

      AbstractOperator(std::shared_ptr<AbstractBanachSpace> domain, std::shared_ptr<AbstractBanachSpace> range);

      virtual ~AbstractOperator();

      virtual std::unique_ptr<AbstractFunctionSpaceElement> operator()(const AbstractFunctionSpaceElement&) const = 0;

      AbstractBanachSpace& getDomain();

      const AbstractBanachSpace& getDomain() const;

      AbstractBanachSpace& getRange();

      const AbstractBanachSpace& getRange() const;

      std::shared_ptr<AbstractBanachSpace> getSharedDomain() const;

      std::shared_ptr<AbstractBanachSpace> getSharedRange() const;

    private:
      std::shared_ptr<AbstractBanachSpace> domain_, range_;
    };
  }
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_OPERATOR_HH
