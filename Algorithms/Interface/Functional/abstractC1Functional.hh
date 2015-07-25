#ifndef ALGORITHM_INTERFACE_ABSTRACT_C1_FUNCTIONAL_HH
#define ALGORITHM_INTERFACE_ABSTRACT_C1_FUNCTIONAL_HH

#include "abstractFunctional.hh"

namespace Algorithm
{
  namespace Interface
  {
    /**
     * @brief Abstract interface for differentiable functionals \f$f:\ X\rightarrow \mathbb{R}\f$.
     */
    class AbstractC1Functional : public AbstractFunctional
    {
    public:
      /**
       * @brief Constructor.
       * @param domain Domain space \f$X\f$.
       */
      AbstractC1Functional(std::shared_ptr<AbstractBanachSpace> domain);

      /// Apply derivative of functional, i.e. compute \f$f'(x) \in X^* \f$.
      virtual std::unique_ptr<AbstractFunctionSpaceElement> d1(const AbstractFunctionSpaceElement& x) const = 0;
    };
  }
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_C1_FUNCTIONAL_HH
