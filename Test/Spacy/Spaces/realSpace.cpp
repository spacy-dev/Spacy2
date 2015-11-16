#include <gtest/gtest.h>

#include "Spacy/vector.hh"
#include "Spacy/Spaces/realSpace.hh"
#include "Spacy/Util/cast.hh"

TEST(RealSpaceTest, DefaultIndex)
{
  auto r = Spacy::Real{2.};
  ASSERT_EQ( r.space()->index() , 0 );
}

TEST(RealSpaceTest,ElementTest)
{
  using namespace Spacy;
  auto R = RealSpace::makeHilbertSpace();
  auto x = R.zeroVector();
  EXPECT_DOUBLE_EQ( toDouble(x) , 0. );
}

TEST(RealSpaceTest,ScalarProductTest)
{
  using namespace Spacy;
  auto R = RealSpace::makeHilbertSpace();
  auto x = R.zeroVector();
  auto y = R.zeroVector();
  toDouble(x) = 1;
  toDouble(y) = -2;
  EXPECT_DOUBLE_EQ( toDouble(x), 1. );
  EXPECT_DOUBLE_EQ( toDouble(y), -2. );
  EXPECT_DOUBLE_EQ( toDouble(x*y), -2. );
  EXPECT_DOUBLE_EQ( toDouble(x*y), toDouble(R.scalarProduct()(x,y)) );
  EXPECT_TRUE( R.isHilbertSpace() );
}

TEST(RealSpaceTest,NormTest)
{
  using namespace Spacy;
  auto R = RealSpace::makeHilbertSpace();
  auto x = R.zeroVector();
  auto y = R.zeroVector();
  toDouble(x) = 1;
  toDouble(y) = -2;
  EXPECT_DOUBLE_EQ( toDouble(x), 1. );
  EXPECT_DOUBLE_EQ( toDouble(y), -2. );
  EXPECT_DOUBLE_EQ( toDouble(R.norm()(x)) , 1. );
  EXPECT_DOUBLE_EQ( toDouble(R.norm()(y)) , 2. );
}
