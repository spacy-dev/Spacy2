#ifndef SPACY_DERIVATIVE_HH
#define SPACY_DERIVATIVE_HH

#include "Spacy/functional.hh"
#include "Spacy/operator.hh"

namespace Spacy
{
  /**
   * @ingroup SpacyGroup
   * @brief Compute the derivative of a functional \f$ f: X\to \mathbb{R} \f$ as (nonlinear) operator \f$ f':X\to X^* \f$.
   * @param f twice differentiable functional
   * @return \f$f'\f$
   * @see @ref C2FunctionalAnchor "C2Functional", @ref C1OperatorAnchor "C1Operator"
   */
  C1Operator derivative(C2Functional f);
}

#endif // SPACY_DERIVATIVE_HH
