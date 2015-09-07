#ifndef SPACY_HESSIAN_HH
#define SPACY_HESSIAN_HH

#include <memory>

#include "Spacy/functional.hh"
#include "Spacy/linearSolver.hh"
#include "Spacy/vector.hh"
#include "Spacy/Util/Base/operatorBase.hh"

namespace Spacy
{
  /**
   * @ingroup SpacyGroup
   * @brief Linear operator \f$f''(x)\f$ for a twice differentiable functional \f$f\f$.
   * @see @ref LinearOperatorAnchor "LinearOperator", @ref Concepts::LinearOperatorConcept "LinearOperatorConcept"
   */
  class Hessian : public OperatorBase
  {
  public:
    /**
     * @brief Constructor.
     * @param F twice differentiable functional
     * @param x point of linearization
     */
    Hessian(C2Functional F, Vector x);

    /**
     * @brief Constructor.
     * @param F twice differentiable functional
     * @param x point of linearization
     * @param solver linear solver to (approximately) compute \f$A(x)^{-1}y\f$
     */
    Hessian(C2Functional F, Vector x, LinearSolver solver);

    /**
     * @brief Apply operator.
     * @param dx point of evaluation
     * @return \f$y=f''(x)dx\f$
     */
    Vector operator ()(const Vector& dx) const;

    /**
     * @brief Access linear solver.
     * @return linear solver representing \f$(f''(x))^{-1}\f$
     */
    LinearSolver solver() const;

  private:
    C2Functional F_;
    Vector x_;
    LinearSolver solver_ = {};
  };
}

#endif // SPACY_HESSIAN_HH
