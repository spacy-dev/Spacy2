// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_ADAPTER_SCALAR_LINEAR_SOLVER_HH
#define SPACY_ADAPTER_SCALAR_LINEAR_SOLVER_HH

namespace Spacy
{
  /// @cond
  class Vector;
  /// @endcond

  /** @addtogroup ScalarGroup @{ */
  namespace Scalar
  {
    class LinearSolver
    {
    public:
      LinearSolver(double value);

      ::Spacy::Vector operator()(const ::Spacy::Vector& y) const;

    private:
      double value_;
    };
  }
  /** @} */
}

#endif // SPACY_ADAPTER_SCALAR_LINEAR_SOLVER_HH
