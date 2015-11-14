#include <gtest/gtest.h>

#include "Test/mockSetup.hh"

using namespace Spacy;

TEST(ProductSpaceCreator,SubSpaces)
{
  auto t = createProductSpaceCreatorWithSpaceIndices();
  ASSERT_EQ( std::get<1>(t) , std::get<0>(t).subSpaces()[0]->index() );
  ASSERT_EQ( std::get<2>(t) , std::get<0>(t).subSpaces()[1]->index() );

  ASSERT_NE( std::get<1>(t) , std::get<0>(t).subSpaces()[1]->index() );
  ASSERT_NE( std::get<2>(t) , std::get<0>(t).subSpaces()[0]->index() );
}

TEST(ProductSpaceCreator, SubSpace)
{
  auto t = createProductSpaceCreatorWithSpaceIndices();
  ASSERT_EQ( std::get<1>(t) , std::get<0>(t).subSpace(0).index() );
  ASSERT_EQ( std::get<2>(t) , std::get<0>(t).subSpace(1).index() );

  ASSERT_NE( std::get<1>(t) , std::get<0>(t).subSpace(1).index() );
  ASSERT_NE( std::get<2>(t) , std::get<0>(t).subSpace(0).index() );
}

//TEST(ProductSpaceCreator, SharedSubSpace)
//{
//  auto t = createProductSpaceCreatorWithSpaceIndices();
//  ASSERT_EQ( std::get<1>(t) , std::get<0>(t).sharedSubSpace(0)->index() );
//  ASSERT_EQ( std::get<2>(t) , std::get<0>(t).sharedSubSpace(1)->index() );

//  ASSERT_NE( std::get<1>(t) , std::get<0>(t).sharedSubSpace(1)->index() );
//  ASSERT_NE( std::get<2>(t) , std::get<0>(t).sharedSubSpace(0)->index() );
//}
