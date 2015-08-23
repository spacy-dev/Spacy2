#ifndef ALGORITHM_LINEARIZED_OPERATOR_HH
#define ALGORITHM_LINEARIZED_OPERATOR_HH

#include "operator.hh"
#include "vector.hh"
#include "Util/Base/operatorBase.hh"

namespace Algorithm
{
  /// Linearized operator \f$A'(x)\f$ of a differentiable operator \f$A\f$.
  class LinearizedOperator : public OperatorBase
  {
  public:
    /**
     * @brief Constructor.
     * @param A (possibly nonlinear) differentiable operator.
     * @param x point of linearization.
     */
    LinearizedOperator(C1Operator A, const Vector& x);

    /// Apply operator, i.e. compute \f$y=A'(x)dx\f$.
    Vector operator ()(const Vector& dx) const;

    /// Get linear solver (representing \f$(A'(x))^{-1}\f$).
    LinearSolver solver() const;

    /// Get linear solver (representing \f$(A'(x))^{-*}\f$).
    //std::unique_ptr<AbstractLinearSolver> adjointSolver() const final override;

  private:
    C1Operator A_;
    Vector x_;
  };
}

#endif // ALGORITHM_LINEARIZED_OPERATOR_HH
