#include <gtest/gtest.h>

#include "../../Algorithms/norm.hh"
#include "../../Algorithms/scalarProduct.hh"
#include "../../Algorithms/FunctionSpaces/RealNumbers/real.hh"
#include "../../Algorithms/FunctionSpaces/RealNumbers/realSpace.hh"
#include "../../Algorithms/FunctionSpaces/RealNumbers/realProduct.hh"
#include "../../Algorithms/spaces.hh"

TEST(RealSpaceTest,ElementTest)
{
  using namespace Algorithm;
  auto x = Spaces::R.element();
  EXPECT_EQ( dynamic_cast<const Real*>(&x.impl()) == nullptr , false );
  EXPECT_DOUBLE_EQ( x.coefficient(0) , 0. );
}

TEST(RealSpaceTest,ScalarProductTest)
{
  using namespace Algorithm;
  auto x = Spaces::R.element();
  auto y = Spaces::R.element();
  x.coefficient(0) = 1;
  y.coefficient(0) = -2;
  EXPECT_DOUBLE_EQ( x.coefficient(0), 1. );
  EXPECT_DOUBLE_EQ( y.coefficient(0), -2. );
  EXPECT_DOUBLE_EQ( x*y, -2. );
  EXPECT_DOUBLE_EQ( x*y, Spaces::R.scalarProduct()(x,y) );
}

TEST(RealSpaceTest,NormTest)
{
  using namespace Algorithm;
  auto x = Spaces::R.element();
  auto y = Spaces::R.element();
  x.coefficient(0) = 1;
  y.coefficient(0) = -2;
  EXPECT_DOUBLE_EQ( x.coefficient(0), 1. );
  EXPECT_DOUBLE_EQ( y.coefficient(0), -2. );
  EXPECT_DOUBLE_EQ( Spaces::R.norm()(x) , 1. );
  EXPECT_DOUBLE_EQ( Spaces::R.norm()(y) , 2. );
}
