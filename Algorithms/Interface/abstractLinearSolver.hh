#ifndef ALGORITHM_INTERFACE_ABSTRACT_LINEAR_SOLVER_HH
#define ALGORITHM_INTERFACE_ABSTRACT_LINEAR_SOLVER_HH

#include <memory>

#include "Interface/Operator/abstractOperator.hh"

namespace Algorithm
{
  namespace Interface
  {
    /// \cond
    class AbstractVectorSpace;
    class AbstractVector;
    /// \endcond

    /**
     * @brief Abstract interface for linear solvers as linear operator \f$X\rightarrow Y\f$.
     */
    class AbstractLinearSolver : public AbstractOperator
    {
    public:
      /**
       * @brief Constructor.
       * @param domain domain space \f$X\f$.
       * @param range range space \f$Y\f$.
       */
      AbstractLinearSolver(std::shared_ptr<AbstractVectorSpace> domain, std::shared_ptr<AbstractVectorSpace> range);

      virtual ~AbstractLinearSolver();

      /// Apply linear solver.
      virtual std::unique_ptr<AbstractVector> operator()(const AbstractVector&) const = 0;

      /// Checks if the linear system is indefinite.
      virtual bool systemIsPositiveDefinite() const;
    };
  }
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_LINEAR_SOLVER_HH
