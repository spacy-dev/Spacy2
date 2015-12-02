// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef MOCK_SCALAR_PRODUCT_HH
#define MOCK_SCALAR_PRODUCT_HH

#include "Spacy/vector.hh"
#include "Spacy/Spaces/realSpace.hh"

#include "norm.hh"

namespace Mock
{
  struct ScalarProduct
  {
    static constexpr int testValue = Norm::testValue * Norm::testValue;
    Spacy::Real operator()(const ::Spacy::Vector&, const ::Spacy::Vector&) const;
  };
}

#endif
