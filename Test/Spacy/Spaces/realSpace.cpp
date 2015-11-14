#include <gtest/gtest.h>

#include "Spacy/vector.hh"
#include "Spacy/Spaces/realSpace.hh"
#include "Spacy/Util/cast.hh"

namespace
{
  double to_double(const Spacy::Vector& x)
  {
    return Spacy::cast_ref<Spacy::Real>(x).impl();
  }

  double& to_double(Spacy::Vector& x)
  {
    return Spacy::cast_ref<Spacy::Real>(x).impl();
  }
}

TEST(RealSpaceTest, DefaultIndex)
{
  auto r = Spacy::Real{2.};
  ASSERT_EQ( r.space()->index() , 0 );
}

TEST(RealSpaceTest,ElementTest)
{
  using namespace Spacy;
  auto R = RealSpace::makeHilbertSpace();
  auto x = R.vector();
  EXPECT_DOUBLE_EQ( to_double(x) , 0. );
}

TEST(RealSpaceTest,ScalarProductTest)
{
  using namespace Spacy;
  auto R = RealSpace::makeHilbertSpace();
  auto x = R.vector();
  auto y = R.vector();
  to_double(x) = 1;
  to_double(y) = -2;
  EXPECT_DOUBLE_EQ( to_double(x), 1. );
  EXPECT_DOUBLE_EQ( to_double(y), -2. );
  EXPECT_DOUBLE_EQ( toDouble(x*y), -2. );
  EXPECT_DOUBLE_EQ( toDouble(x*y), toDouble(R.scalarProduct()(x,y)) );
  EXPECT_TRUE( R.isHilbertSpace() );
}

TEST(RealSpaceTest,NormTest)
{
  using namespace Spacy;
  auto R = RealSpace::makeHilbertSpace();
  auto x = R.vector();
  auto y = R.vector();
  to_double(x) = 1;
  to_double(y) = -2;
  EXPECT_DOUBLE_EQ( to_double(x), 1. );
  EXPECT_DOUBLE_EQ( to_double(y), -2. );
  EXPECT_DOUBLE_EQ( toDouble(R.norm()(x)) , 1. );
  EXPECT_DOUBLE_EQ( toDouble(R.norm()(y)) , 2. );
}
