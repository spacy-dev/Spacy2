// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "norm.hh"

Spacy::Real Mock::Norm::operator()(const ::Spacy::Vector&) const
{
  return Spacy::Real(testValue);
}

Spacy::Real Mock::Norm10::operator()(const ::Spacy::Vector&) const
{
  return Spacy::Real(testValue);
}

