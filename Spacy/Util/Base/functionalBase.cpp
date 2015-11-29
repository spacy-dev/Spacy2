// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "functionalBase.hh"

#include "Spacy/vectorSpace.hh"

namespace Spacy
{
  FunctionalBase::FunctionalBase(const VectorSpace& domain)
    : domain_(domain)
  {}

  const VectorSpace& FunctionalBase::domain() const
  {
    return domain_;
  }
}
