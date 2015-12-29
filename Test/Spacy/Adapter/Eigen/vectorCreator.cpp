// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include <gtest/gtest.h>

#include "Spacy/Adapter/Eigen/vectorCreator.hh"

#include "Test/mockSetup.hh"
#include "setup.hh"

using namespace Spacy;

TEST(Rn,SingleSpaceCreator)
{
  auto V = Rn::makeHilbertSpace( testDim() );
  ASSERT_EQ( creator<Rn::VectorCreator>(V).dim() , testDim() );
}
