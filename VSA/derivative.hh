#ifndef ALGORITHM_DERIVATIVE_HH
#define ALGORITHM_DERIVATIVE_HH

#include "VSA/functional.hh"
#include "VSA/operator.hh"
#include "VSA/vector.hh"

namespace VSA
{
  /**
   * @ingroup VSpaceGroup
   * @brief Compute the derivative of a functional \f$ f: X\to \mathbb{R} \f$ as (nonlinear) operator \f$ f':X\to X^* \f$.
   * @param f twice differentiable functional
   * @return \f$f'\f$
   * @see @ref C2FunctionalAnchor "C2Functional", @ref C1OperatorAnchor "C1Operator"
   */
  C1Operator derivative(C2Functional f);

  /**
   * @ingroup VSpaceGroup
   * @brief Compute the linearization of a functional \f$ f: X\to \mathbb{R} \f$ at \f$x\in X\f$ as linear functional \f$ f'(x):X\to\mathbb{R} \f$.
   * @param f twice differentiable functional
   * @param x point of linearization
   * @return \f$f'(x)\f$
   * @see @ref C2FunctionalAnchor "C2Functional", @ref FunctionalAnchor "Functional"
   */
  Functional linearize(C2Functional f, Vector x);
}

#endif // ALGORITHM_DERIVATIVE_HH
