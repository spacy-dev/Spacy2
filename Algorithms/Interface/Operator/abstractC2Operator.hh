#ifndef ALGORITHM_INTERFACE_ABSTRACT_C2_OPERATOR_HH
#define ALGORITHM_INTERFACE_ABSTRACT_C2_OPERATOR_HH

#include <memory>

#include "abstractC1Operator.hh"

namespace Algorithm
{
  namespace Interface
  {
    /// \cond
    class AbstractLinearizedOperator;
    /// \endcond

    /**
     * @brief Abstract interface for twice differentiable operators \f$A:\ X\rightarrow Y\f$, between function spaces \f$X\f$ and \f$Y\f$.
     */
    class AbstractC2Operator : public AbstractC1Operator
    {
    public:
      /**
       * @brief Constructor.
       * @param domain domain space \f$X\f$.
       * @param range range space \f$Y\f$.
       */
      AbstractC2Operator(std::shared_ptr<AbstractFunctionSpace> domain, std::shared_ptr<AbstractFunctionSpace> range);

      /// Apply second derivative of operator, i.e. compute \f$y=A''(x)(dx,dy)\f$.
      virtual std::unique_ptr<AbstractFunctionSpaceElement> d2(const AbstractFunctionSpaceElement& x,
                                                               const AbstractFunctionSpaceElement& dx,
                                                               const AbstractFunctionSpaceElement& dy) const = 0;
    };
  }
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_C2_OPERATOR_HH
