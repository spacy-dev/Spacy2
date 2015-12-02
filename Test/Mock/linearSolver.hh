// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_MOCK_LINEAR_SOLVER_HH
#define SPACY_MOCK_LINEAR_SOLVER_HH

namespace Spacy
{
  class Vector;
}

namespace Mock
{
  struct IndefiniteLinearSolver
  {
    Spacy::Vector operator()(const Spacy::Vector& x) const;

    bool isPositiveDefinite() const;
  };
}

#endif // SPACY_MOCK_LINEAR_SOLVER_HH
