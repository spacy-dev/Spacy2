// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include <cmath>

#include <gtest/gtest.h>

#include "Test/mockSetup.hh"
#include "Spacy/Spaces/ProductSpace/norm.hh"

using namespace Spacy;

TEST(ProductSpaceNorm,Apply)
{
  auto norm = ProductSpace::Norm{};
  auto V = makeProductHilbertSpace();

  auto v = std::get<0>(V).zeroVector();

  ASSERT_EQ( norm(v) , sqrt(2*Mock::Norm::testValue*Mock::Norm::testValue) );
}

