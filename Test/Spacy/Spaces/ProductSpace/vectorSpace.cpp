#include <Test/gtest.hh>

#include <Test/mockSetup.hh>

#include <cmath>

using namespace Spacy;

TEST(ProductSpace,Index)
{
    auto V = makeProductHilbertSpace();
    EXPECT_EQ( std::get<0>(V).index() , std::get<2>(V)+1u );
    EXPECT_EQ( std::get<0>(V).index() , std::get<1>(V)+2u );
    EXPECT_EQ( creator<ProductSpace::VectorCreator>(std::get<0>(V)).idMap(0) , 0u );
    EXPECT_EQ( creator<ProductSpace::VectorCreator>(std::get<0>(V)).idMap(1) , 1u );
}

TEST(ProductSpace, HasId)
{
  auto V = makeProductHilbertSpaceWithMap();
  EXPECT_TRUE( creator<ProductSpace::VectorCreator>(std::get<0>(V)).hasId(firstGlobalId()) );
  EXPECT_TRUE( creator<ProductSpace::VectorCreator>(std::get<0>(V)).hasId(secondGlobalId()) );
  EXPECT_FALSE( creator<ProductSpace::VectorCreator>(std::get<0>(V)).hasId(notGlobalId()) );
}

TEST(ProductSpace, IdMap)
{
  auto V = makeProductHilbertSpaceWithMap();
  EXPECT_EQ( creator<ProductSpace::VectorCreator>(std::get<0>(V)).idMap(firstGlobalId()) , 0u );
  EXPECT_EQ( creator<ProductSpace::VectorCreator>(std::get<0>(V)).idMap(secondGlobalId()) , 1u );
}

TEST(ProductSpace,Vector)
{
  auto V = makeProductHilbertSpace();
  auto v = zero(std::get<0>(V));
  EXPECT_NO_THROW( cast_ref<ProductSpace::Vector>(v) );
}

TEST(ProductSpace, IsHilbertSpace)
{
  auto V = makeProductHilbertSpace();
  EXPECT_TRUE( std::get<0>(V).isHilbertSpace() );
}

TEST(ProductSpace, Norm)
{
  auto V = makeProductHilbertSpace();
  auto v = zero(std::get<0>(V));
  EXPECT_EQ( std::get<0>(V).norm()(v) , sqrt(2*Mock::ScalarProduct::testValue) );
}

TEST(ProductSpace, ScalarProduct)
{
  auto V = makeProductHilbertSpace();
  auto v = zero(std::get<0>(V));
  ASSERT_EQ( std::get<0>(V).scalarProduct()(v,v) , 2.*Mock::ScalarProduct::testValue );
}
