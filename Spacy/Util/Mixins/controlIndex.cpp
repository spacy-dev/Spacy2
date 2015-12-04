// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "controlIndex.hh"

#include <cmath>

Spacy::Mixin::ControlIndex::ControlIndex(unsigned index) noexcept
  : index_(index)
{}

void Spacy::Mixin::ControlIndex::setControlIndex(unsigned index)
{
  index_ = index;
  notify();
}

double Spacy::Mixin::ControlIndex::controlIndex() const noexcept
{
  return index_;
}

void Spacy::Mixin::ControlIndex::update(ControlIndex* changedSubject)
{
  setControlIndex( changedSubject->controlIndex() );
}
