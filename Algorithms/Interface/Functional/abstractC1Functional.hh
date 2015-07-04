#ifndef ALGORITHM_INTERFACE_ABSTRACT_C1_FUNCTIONAL_HH
#define ALGORITHM_INTERFACE_ABSTRACT_C1_FUNCTIONAL_HH

#include "abstractFunctional.hh"
#include "Interface/Operator/linearizedOperator.hh"

namespace Algorithm
{
  namespace Interface
  {
    class AbstractC1Operator;

    class AbstractC1Functional : public AbstractFunctional
    {
    public:
      AbstractC1Functional(std::shared_ptr<AbstractBanachSpace> domain);

      virtual double d1(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& dx) const = 0;
    };
  }
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_C1_FUNCTIONAL_HH
