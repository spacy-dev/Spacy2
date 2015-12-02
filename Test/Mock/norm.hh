// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef MOCK_NORM_HH
#define MOCK_NORM_HH

#include "Spacy/vector.hh"
#include "Spacy/Spaces/realSpace.hh"

namespace Mock
{
  struct Norm
  {
    static constexpr int testValue = 5;
    Spacy::Real operator()(const ::Spacy::Vector&) const;
  };

  struct Norm10
  {
    static constexpr int testValue = 10;
    Spacy::Real operator()(const ::Spacy::Vector&) const;
  };
}

#endif
