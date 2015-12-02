// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "Spacy/vector.hh"
#include "Spacy/vectorSpace.hh"

#include "vectorCreator.hh"
#include "vector.hh"

Mock::Vector Mock::VectorCreator::operator()(const Spacy::VectorSpace* space) const
{
  return Mock::Vector{*space};
}
