#ifndef ALGORITHM_INTERFACE_LINEARIZED_OPERATOR_HH
#define ALGORITHM_INTERFACE_LINEARIZED_OPERATOR_HH

#include <memory>

#include "vector.hh"
#include "abstractOperator.hh"
#include "abstractLinearOperator.hh"

namespace Algorithm
{
  namespace Interface
  {
    /// Linearized operator \f$A'(x)\f$ of a differentiable operator \f$A\f$.
    class LinearizedOperator : public AbstractLinearOperator
    {
    public:
      /**
       * @brief Constructor.
       * @param A (possibly nonlinear) differentiable operator.
       * @param x point of linearization.
       */
      LinearizedOperator(std::unique_ptr<AbstractOperator>&& A, const Vector& x);

      /// Apply operator, i.e. compute \f$y=A'(x)dx\f$.
      Vector operator ()(const Vector& dx) const final override;

      /// Get linear solver (representing \f$(A'(x))^{-1}\f$).
      std::unique_ptr<AbstractLinearSolver> solver() const final override;

      /// Get linear solver (representing \f$(A'(x))^{-*}\f$).
      std::unique_ptr<AbstractLinearSolver> adjointSolver() const final override;

    private:
      LinearizedOperator* cloneImpl() const;

      std::unique_ptr<AbstractOperator> A_;
      Vector x_;
    };
  }
}

#endif // ALGORITHM_INTERFACE_LINEARIZED_OPERATOR_HH
