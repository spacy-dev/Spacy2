#ifndef ALGORITHM_INTERFACE_ABSTRACT_LINEAR_OPERATOR_HH
#define ALGORITHM_INTERFACE_ABSTRACT_LINEAR_OPERATOR_HH

#include <memory>

#include "abstractOperator.hh"

namespace Algorithm
{
  namespace Interface
  {
    /// \cond
    class AbstractBanachSpace;
    class AbstractLinearSolver;
    /// \endcond

    /**
     * @brief Abstract interface for linear operators. Provides access to a linear solver.
     */
    class AbstractLinearOperator : public AbstractOperator
    {
    public:
      /**
       * @brief Constructor.
       * @param domain domain space \f$X\f$.
       * @param range range space \f$Y\f$.
       */
      AbstractLinearOperator(std::shared_ptr<AbstractBanachSpace> domain, std::shared_ptr<AbstractBanachSpace> range);

      /// Get linear solver.
      virtual std::unique_ptr<AbstractLinearSolver> solver() const = 0;
    };
  }
}
#endif // ALGORITHM_INTERFACE_ABSTRACT_LINEAR_OPERATOR_HH
