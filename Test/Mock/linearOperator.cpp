// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "linearOperator.hh"

#include "Spacy/Spaces/realSpace.hh"
#include "linearSolver.hh"

namespace Mock
{
  LinearOperator::LinearOperator(double value)
    : Spacy::Mixin::Get<double>(value),
      Spacy::OperatorBase(Spacy::Space::R,Spacy::Space::R),
      Spacy::VectorBase(Spacy::Space::R)
  {}

  ::Spacy::Vector LinearOperator::operator()(const ::Spacy::Vector& dx) const
  {
    return Spacy::Real(get());
  }

  ::Spacy::Real LinearOperator::operator()(const LinearOperator& y) const
  {
    return Spacy::Real(get()*y.get());
  }

  /// Access solver representing \f$A^{-1}\f$
  ::Spacy::LinearSolver LinearOperator::solver() const
  {
    return IndefiniteLinearSolver();
  }
}
