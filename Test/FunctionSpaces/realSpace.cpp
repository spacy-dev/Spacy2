#include <gtest/gtest.h>

#include "norm.hh"
#include "scalarProduct.hh"
#include "FunctionSpaces/RealNumbers/real.hh"
#include "FunctionSpaces/RealNumbers/realSpace.hh"
#include "FunctionSpaces/RealNumbers/realProduct.hh"
#include "Util/cast.hh"

TEST(RealSpaceTest,ElementTest)
{
  using namespace Algorithm;
  auto R = makeRealSpace();
  auto x = R.element();
  EXPECT_DOUBLE_EQ( static_cast<double>(cast_ref<Real>(x)) , 0. );
}

TEST(RealSpaceTest,ScalarProductTest)
{
  using namespace Algorithm;
  auto R = makeRealSpace();
  auto x = R.element();
  auto y = R.element();
  cast_ref<Real&>(x) = 1;
  cast_ref<Real&>(y) = -2;
  EXPECT_DOUBLE_EQ( cast_ref<Real>(x), 1. );
  EXPECT_DOUBLE_EQ( cast_ref<Real>(y), -2. );
  EXPECT_DOUBLE_EQ( x*y, -2. );
  EXPECT_DOUBLE_EQ( x*y, R.scalarProduct()(x,y) );
}

TEST(RealSpaceTest,NormTest)
{
  using namespace Algorithm;
  auto R = makeRealSpace();
  auto x = R.element();
  auto y = R.element();
  cast_ref<Real&>(x) = 1;
  cast_ref<Real&>(y) = -2;
  EXPECT_DOUBLE_EQ( cast_ref<Real>(x), 1. );
  EXPECT_DOUBLE_EQ( cast_ref<Real>(y), -2. );
  EXPECT_DOUBLE_EQ( R.norm()(x) , 1. );
  EXPECT_DOUBLE_EQ( R.norm()(y) , 2. );
}
