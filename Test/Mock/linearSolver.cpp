// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "linearSolver.hh"

#include "Spacy/vector.hh"

namespace Mock
{
  Spacy::Vector IndefiniteLinearSolver::operator ()(const Spacy::Vector& x) const
  {
    return x;
  }

  bool IndefiniteLinearSolver::isPositiveDefinite() const
  {
    return true;
  }
}
