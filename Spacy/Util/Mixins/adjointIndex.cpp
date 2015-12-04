// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "adjointIndex.hh"

#include <cmath>

Spacy::Mixin::AdjointIndex::AdjointIndex(unsigned index) noexcept
  : index_(index)
{}

void Spacy::Mixin::AdjointIndex::setAdjointIndex(unsigned index)
{
  index_ = index;
  notify();
}

double Spacy::Mixin::AdjointIndex::adjointIndex() const noexcept
{
  return index_;
}

void Spacy::Mixin::AdjointIndex::update(AdjointIndex* changedSubject)
{
  setAdjointIndex( changedSubject->adjointIndex() );
}
