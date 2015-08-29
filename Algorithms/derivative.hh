#ifndef ALGORITHM_DERIVATIVE_HH
#define ALGORITHM_DERIVATIVE_HH

#include "Algorithms/functional.hh"
#include "Algorithms/operator.hh"

namespace Algorithm
{
  /**
   * @ingroup VHatGroup
   * @brief Compute the derivative of a functional \f$ f: X\to \mathbb{R} \f$ as (nonlinear) operator \f$ f':X\to X^* \f$.
   * @param f twice differentiable functional
   * @return \f$f'\f$
   * @see @ref C2FunctionalAnchor "C2Functional", @ref C1OperatorAnchor "C1Operator"
   */
  C1Operator derivative(C2Functional f);
}

#endif // ALGORITHM_DERIVATIVE_HH
