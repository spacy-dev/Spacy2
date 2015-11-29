// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_DERIVATIVE_HH
#define SPACY_DERIVATIVE_HH

namespace Spacy
{
  /// @cond
  class C1Operator;
  class C2Functional;
  /// @endcond

  /**
   * @ingroup SpacyGroup
   * @brief Compute the derivative of a functional \f$ f: X\to \mathbb{R} \f$ as (nonlinear) operator \f$ f':X\to X^* \f$.
   * @param f twice differentiable functional
   * @return \f$f'\f$
   * @see C2Functional
   */
  C1Operator derivative(C2Functional f);
}

#endif // SPACY_DERIVATIVE_HH
