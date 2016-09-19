#pragma once

#include <Spacy/c1Operator.hh>
#include <Spacy/c2Functional.hh>

namespace Spacy
{
  /**
   * @brief Compute the derivative of a functional \f$ f: X\to \mathbb{R} \f$ as (nonlinear) operator \f$ f':X\to X^* \f$.
   * @param f twice differentiable functional
   * @return \f$f'\f$
   */
  C1Operator derivative(C2Functional f);
}
