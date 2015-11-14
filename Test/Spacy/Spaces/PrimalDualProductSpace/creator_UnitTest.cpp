//#include <gtest/gtest.h>

//#include "Test/mockSetup.hh"

//using namespace Spacy;

//TEST(PrimalDualProductSpaceCreator,SubSpace)
//{
//  auto t = makePrimalDualProductSpaceCreatorWithSpaceIndices();
//  ASSERT_EQ( std::get<1>(t) , std::get<0>(t).primalSubSpace().index() );
//  ASSERT_EQ( std::get<2>(t) , std::get<0>(t).dualSubSpace().index() );

//  ASSERT_NE( std::get<1>(t) , std::get<0>(t).dualSubSpace().index() );
//  ASSERT_NE( std::get<2>(t) , std::get<0>(t).primalSubSpace().index() );
//}

//TEST(PrimalDualProductSpaceCreator, SharedSubSpace)
//{
//  auto t = makePrimalDualProductSpaceCreatorWithSpaceIndices();
//  ASSERT_EQ( std::get<1>(t) , std::get<0>(t).sharedPrimalSubSpace()->index() );
//  ASSERT_EQ( std::get<2>(t) , std::get<0>(t).sharedDualSubSpace()->index() );

//  ASSERT_NE( std::get<1>(t) , std::get<0>(t).sharedDualSubSpace()->index() );
//  ASSERT_NE( std::get<2>(t) , std::get<0>(t).sharedPrimalSubSpace()->index() );
//}
