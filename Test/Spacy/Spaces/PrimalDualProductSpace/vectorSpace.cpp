// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

//#include <cmath>
//#include <gtest/gtest.h>

//#include "Test/mockSetup.hh"

//using namespace Spacy;

//TEST(PrimalDualProductSpace,CreateVector)
//{
//  auto space = makePrimalDualProductHilbertSpace();
//  auto vector = space.vector();
//  ASSERT_NO_THROW( cast_ref<PrimalDualProductSpace::Vector>(vector) );
//}

//TEST(PrimalDualProductSpace, IsHilbertSpace)
//{
//  auto space = makePrimalDualProductHilbertSpace();
//  ASSERT_TRUE( space.isHilbertSpace() );
//}

//TEST(PrimalDualProductSpace, Norm)
//{
//  auto space = makePrimalDualProductHilbertSpace();
//  auto vector = space.vector();
//  ASSERT_EQ( space.norm()(vector) , sqrt(3*Mock::ScalarProduct::testValue) );
//}

//TEST(PrimalDualProductSpace, ScalarProduct)
//{
//  auto space = makePrimalDualProductHilbertSpace();
//  auto vector = space.vector();
//  ASSERT_EQ( space.scalarProduct()(vector,vector) , 3.*Mock::ScalarProduct::testValue );
//}
