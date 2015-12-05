// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_ALGORITM_SCALAR_QUADRATIC_FUNCTION_HH
#define SPACY_ALGORITM_SCALAR_QUADRATIC_FUNCTION_HH

#include "Spacy/Spaces/RealSpace/real.hh"

namespace Spacy
{
  ///A one-dimensional quadratic function \f$q(t) = a + bt + ct^2\f$.
  class Quadratic
  {
  public:
    Quadratic(Real a, Real b, Real c) noexcept;

    /// Compute \f$q(t) = a + bt + ct^2 \f$.
    Real operator()(Real t) const noexcept;

  private:
    Real a_, b_, c_;
  };
}

#endif // SPACY_ALGORITM_SCALAR_QUADRATIC_FUNCTION_HH
