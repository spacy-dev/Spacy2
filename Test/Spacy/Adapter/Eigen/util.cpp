// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include <gtest/gtest.h>

#include "Spacy/Adapter/Eigen/util.hh"
#include "Spacy/Adapter/Eigen/vectorCreator.hh"

#include "Test/mockSetup.hh"
#include "setup.hh"

using namespace Spacy;

TEST(Rn,GetSize)
{
  auto V = Rn::makeHilbertSpace(testDim());
  auto x = V.zeroVector();
  ASSERT_EQ( Rn::getSize(x) , testDim() );
}

TEST(Rn,GetSizeOfProductSpace)
{
  auto V = Rn::makeHilbertSpace( {testDim(),testDim(),testDim()} );
  auto x = V.zeroVector();
  ASSERT_EQ( Rn::getSize(x) , 3*testDim() );
}

TEST(Rn,GetSizeOfPrimalDualProductSpace)
{
  auto V = Rn::makeHilbertSpace( {testDim(),testDim(),testDim()} , {1,2} , {0} );
  auto x = V.zeroVector();
  ASSERT_EQ( Rn::getSize(x) , 3*testDim() );
}
