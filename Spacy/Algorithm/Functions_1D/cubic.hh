// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_1D_MODEL_CUBIC_FUNCTION_HH
#define SPACY_1D_MODEL_CUBIC_FUNCTION_HH

#include "Spacy/Spaces/RealSpace/real.hh"

namespace Spacy
{
  namespace Functions_1D
  {
    /// A one-dimensional cubic function \f$q(t) = a + bt + ct^2 + dt^3\f$.
    class Cubic
    {
    public:
      Cubic(Real a, Real b, Real c, Real d) noexcept;

      /// Compute \f$q(t) = a + bt + ct^2 + dt^3 \f$.
      Real operator()(Real t) const noexcept;

    private:
      Real a_, b_, c_, d_;
    };
  }
}

#endif // SPACY_1D_MODEL_CUBIC_FUNCTION_HH
