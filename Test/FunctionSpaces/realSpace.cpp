#include <gtest/gtest.h>

#include "Algorithms/vector.hh"
#include "Algorithms/VectorSpaces/realSpace.hh"
#include "Algorithms/Util/cast.hh"

TEST(RealSpaceTest,ElementTest)
{
  using namespace Algorithm;
  auto R = Real::makeHilbertSpace();
  auto x = R.vector();
  EXPECT_DOUBLE_EQ( static_cast<double>(cast_ref<Real::Vector>(x)) , 0. );
}

TEST(RealSpaceTest,ScalarProductTest)
{
  using namespace Algorithm;
  auto R = Real::makeHilbertSpace();
  auto x = R.vector();
  auto y = R.vector();
  cast_ref<Real::Vector>(x) = 1;
  cast_ref<Real::Vector>(y) = -2;
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(x), 1. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(y), -2. );
  EXPECT_DOUBLE_EQ( x*y, -2. );
  EXPECT_DOUBLE_EQ( x*y, R.scalarProduct()(x,y) );
}

TEST(RealSpaceTest,NormTest)
{
  using namespace Algorithm;
  auto R = Real::makeHilbertSpace();
  auto x = R.vector();
  auto y = R.vector();
  cast_ref<Real::Vector>(x) = 1;
  cast_ref<Real::Vector>(y) = -2;
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(x), 1. );
  EXPECT_DOUBLE_EQ( cast_ref<Real::Vector>(y), -2. );
  EXPECT_DOUBLE_EQ( R.norm()(x) , 1. );
  EXPECT_DOUBLE_EQ( R.norm()(y) , 2. );
}
