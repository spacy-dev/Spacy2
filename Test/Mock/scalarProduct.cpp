// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "scalarProduct.hh"

Spacy::Real Mock::ScalarProduct::operator()(const ::Spacy::Vector&, const ::Spacy::Vector&) const
{
  return Spacy::Real(testValue);
}

