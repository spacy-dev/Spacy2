// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "linearSolver.hh"

#include "Spacy/vectorSpace.hh"

namespace Spacy
{
  Vector IndefiniteLinearSolver::operator()(const Vector& x) const
  {
    return base_->operator()(x);
  }

  bool IndefiniteLinearSolver::isPositiveDefinite() const
  {
    return base_->isPositiveDefinite();
  }

  IndefiniteLinearSolver::operator bool() const
  {
    return base_;
  }
}
