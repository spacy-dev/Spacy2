// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_ALGORITHM_FUNCTIONS_1D_FIND_GLOBAL_MINIMIZER_HH
#define SPACY_ALGORITHM_FUNCTIONS_1D_FIND_GLOBAL_MINIMIZER_HH

#include <functional>
#include "Spacy/Spaces/RealSpace/real.hh"

namespace Spacy
{
  /// @cond
  class Real;
  /// @endcond

  /**
   * @brief Find global minimizer of \f$f\f$ in \f$[a,b]\f$.
   * @param f Nonlinear function \f$ f: \mathbb{R}\to\mathbb{R} \f$
   * @param a lower bound of the admissible interval
   * @param b upper bound of the admissible interval
   * @param eps relative accuracy
   * @return \f$ x \in \mathrm{argmin}_{[a,b]} \f$
   */
  Real findGlobalMinimizer(const std::function<Real(const Real)>& f, Real a, Real b, Real eps = 1e-2);
}

#endif // SPACY_ALGORITHM_FUNCTIONS_1D_FIND_GLOBAL_MINIMIZER_HH
