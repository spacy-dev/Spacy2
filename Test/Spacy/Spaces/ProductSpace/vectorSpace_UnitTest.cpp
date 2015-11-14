#include <cmath>
#include <gtest/gtest.h>

#include "Test/mockSetup.hh"

using namespace Spacy;

TEST(ProductSpace,Index)
{
  auto space = makeProductHilbertSpace();
  ASSERT_EQ( std::get<0>(space).index() , std::get<2>(space)+1u );
  ASSERT_EQ( std::get<0>(space).index() , std::get<1>(space)+2u );
  ASSERT_EQ( creator<ProductSpace::VectorCreator>(std::get<0>(space)).idMap(0) , 0u );
  ASSERT_EQ( creator<ProductSpace::VectorCreator>(std::get<0>(space)).idMap(1) , 1u );
}

TEST(ProductSpace, HasId)
{
  auto space = makeProductHilbertSpaceWithMap();
  ASSERT_TRUE( creator<ProductSpace::VectorCreator>(std::get<0>(space)).hasId(firstGlobalId()) );
  ASSERT_TRUE( creator<ProductSpace::VectorCreator>(std::get<0>(space)).hasId(secondGlobalId()) );
  ASSERT_FALSE( creator<ProductSpace::VectorCreator>(std::get<0>(space)).hasId(notGlobalId()) );
}

TEST(ProductSpace, IdMap)
{
  auto space = makeProductHilbertSpaceWithMap();
  ASSERT_EQ( creator<ProductSpace::VectorCreator>(std::get<0>(space)).idMap(firstGlobalId()) , 0u );
  ASSERT_EQ( creator<ProductSpace::VectorCreator>(std::get<0>(space)).idMap(secondGlobalId()) , 1u );
}

TEST(ProductSpace,Vector)
{
  auto space = makeProductHilbertSpace();
  auto vector = std::get<0>(space).vector();
  ASSERT_NO_THROW( cast_ref<ProductSpace::Vector>(vector) );
}

TEST(ProductSpace, IsHilbertSpace)
{
  auto space = makeProductHilbertSpace();
  ASSERT_TRUE( std::get<0>(space).isHilbertSpace() );
}

TEST(ProductSpace, Norm)
{
  auto space = makeProductHilbertSpace();
  auto vector = std::get<0>(space).vector();
  ASSERT_EQ( std::get<0>(space).norm()(vector) , sqrt(2*Mock::ScalarProduct::testValue) );
}

TEST(ProductSpace, ScalarProduct)
{
  auto space = makeProductHilbertSpace();
  auto vector = std::get<0>(space).vector();
  ASSERT_EQ( std::get<0>(space).scalarProduct()(vector,vector) , 2.*Mock::ScalarProduct::testValue );
}
