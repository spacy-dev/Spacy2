#ifndef ALGORITHM_INTERFACE_ABSTRACT_C1_OPERATOR_HH
#define ALGORITHM_INTERFACE_ABSTRACT_C1_OPERATOR_HH

#include <memory>

#include "abstractOperator.hh"

namespace Algorithm
{
  namespace Interface
  {
    /// \cond
    class AbstractLinearizedOperator;
    class AbstractLinearSolver;
    class LinearizedOperator;
    /// \endcond

    /**
     * @brief Abstract interface for differentiable operators \f$A:\ X\rightarrow Y\f$, between function spaces \f$X\f$ and \f$Y\f$.
     */
    class AbstractC1Operator : public AbstractOperator
    {
    public:
      /**
       * @brief Constructor.
       * @param domain domain space \f$X\f$.
       * @param range range space \f$Y\f$.
       */
      AbstractC1Operator(std::shared_ptr<AbstractBanachSpace> domain, std::shared_ptr<AbstractBanachSpace> range);

      /// Get linearization \f$A'(x): X \rightarrow Y\f$.
      std::unique_ptr<LinearizedOperator> linearization(const AbstractFunctionSpaceElement& x) const;

      /// Apply first derivative of operator, i.e. compute \f$y=A'(x)dx\f$.
      virtual std::unique_ptr<AbstractFunctionSpaceElement> d1(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& dx) const = 0;

    protected:
      friend class LinearizedOperator;

      virtual std::unique_ptr<LinearizedOperator> makeLinearization(const AbstractFunctionSpaceElement& x) const;

      virtual std::unique_ptr<AbstractLinearSolver> makeSolver() const;
    };
  }
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_C1_OPERATOR_HH
