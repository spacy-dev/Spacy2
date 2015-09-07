#ifndef SPACY_LINEARIZED_OPERATOR_HH
#define SPACY_LINEARIZED_OPERATOR_HH

#include "Spacy/linearSolver.hh"
#include "Spacy/operator.hh"
#include "Spacy/vector.hh"
#include "Spacy/Util/Base/operatorBase.hh"

namespace Spacy
{
  /**
   * @ingroup SpacyGroup
   * @brief Linearized operator \f$A'(x)\f$ of a differentiable operator \f$A\f$.
   * @see @ref LinearOperatorAnchor "LinearOperator", @ref Concepts::LinearOperatorConcept "LinearOperatorConcept"
   */
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

    /**
     * @brief Apply operator.
     * @param dx point of evaluation
     * @return\f$y=A'(x)dx\f$
     */
    Vector operator ()(const Vector& dx) const;

    /**
     * @brief Access linear solver.
     * @return linear solver representing \f$(A'(x))^{-1}\f$
     */
    LinearSolver solver() const;

    // Get linear solver (representing \f$(A'(x))^{-*}\f$).
    //std::unique_ptr<AbstractLinearSolver> adjointSolver() const final override;

  private:
    C1Operator A_;
    Vector x_;
    LinearSolver solver_ = {};
  };
}

#endif // SPACY_LINEARIZED_OPERATOR_HH
