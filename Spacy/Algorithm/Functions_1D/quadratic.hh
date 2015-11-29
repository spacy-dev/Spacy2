// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_FUNCTIONS_1D_QUADRATIC_FUNCTION_HH
#define SPACY_FUNCTIONS_1D_QUADRATIC_FUNCTION_HH

#include "Spacy/Spaces/RealSpace/real.hh"

namespace Spacy
{
  namespace Functions_1D
  {
    /**
     * @ingroup SpacyGroup
     * @brief A one-dimensional quadratic function \f$q(t) = a + bt + ct^2\f$.
     */
    class Quadratic
    {
    public:
      /**
       * @brief Constructor.
       * @param a coefficient of constant term
       * @param b coefficient of linear term
       * @param c coefficient of quadratic term
       */
      Quadratic(Real a, Real b, Real c) noexcept;

      /**
       * @brief Compute \f$q(t) = a + bt + ct^2 \f$.
       * @param t argument
       * @return \f$q(t) = a + bt + ct^2 \f$
       */
      Real operator()(Real t) const noexcept;

    private:
      Real a_, b_, c_;
    };
  }
}

#endif // SPACY_FUNCTIONS_1D_QUADRATIC_FUNCTION_HH
