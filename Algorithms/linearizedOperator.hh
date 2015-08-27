#ifndef ALGORITHM_LINEARIZED_OPERATOR_HH
#define ALGORITHM_LINEARIZED_OPERATOR_HH

#include "linearSolver.hh"
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
     * @param A differentiable operator.
     * @param x point of linearization.
     */
    LinearizedOperator(C1Operator A, Vector x);

    /**
     * @brief Constructor.
     * @param A differentiable operator.
     * @param x point of linearization.
     * @param solver linear solver to (approximately) compute \f$A(x)^{-1}y\f$
     */
    LinearizedOperator(C1Operator A,  Vector x, LinearSolver solver);

    /// Apply operator, i.e. compute \f$y=A'(x)dx\f$.
    Vector operator ()(const Vector& dx) const;

    /// Get linear solver (representing \f$(A'(x))^{-1}\f$).
    const LinearSolver& solver() const;

    /// Get linear solver (representing \f$(A'(x))^{-*}\f$).
    //std::unique_ptr<AbstractLinearSolver> adjointSolver() const final override;

  private:
    C1Operator A_;
    Vector x_;
    LinearSolver solver_;
  };
}

#endif // ALGORITHM_LINEARIZED_OPERATOR_HH
