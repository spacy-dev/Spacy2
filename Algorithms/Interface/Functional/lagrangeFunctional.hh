#ifndef ALGORITHM_INTERFACE_ABSTRACT_LAGRANGE_FUNCTIONAL_HH
#define ALGORITHM_INTERFACE_ABSTRACT_LAGRANGE_FUNCTIONAL_HH

#include "abstractC2Functional.hh"

namespace Algorithm
{
  namespace Interface
  {
    class AbstractBanachSpace;
    class AbstractC1Operator;

    class AbstractLagrangeFunctional : public AbstractC2Functional
    {
    public:
      AbstractLagrangeFunctional(std::shared_ptr<AbstractBanachSpace> domain);

      virtual const Interface::AbstractC2Functional& costFunctional() const = 0;

      virtual const Interface::AbstractC1Operator& constraint() const = 0;
    };
  }
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_LAGRANGE_FUNCTIONAL_HH
