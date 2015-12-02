#include <gtest/gtest.h>

#include "Spacy/linearOperator.hh"
#include "Spacy/Spaces/realSpace.hh"
#include "Test/mockSetup.hh"
#include "Test/Mock/linearOperator.hh"
#include "Test/Mock/linearSolver.hh"

using namespace Spacy;

namespace
{
  void test(const LinearOperator& f, double value)
  {
    EXPECT_EQ( toDouble(f(Space::R.zeroVector())) , value );
    EXPECT_EQ( 0u , f.domain().index() );
    EXPECT_EQ( 0u , f.range().index() );

    auto L = f.solver();
    auto expected = Spacy::target<Mock::IndefiniteLinearSolver>(L) != nullptr;
    EXPECT_TRUE(expected);
  }
}

TEST(LinearOperator,IsEmpty)
{
  LinearOperator f;
  LinearOperator g = Mock::LinearOperator(1);

  bool f_is_empty = !f;
  bool g_is_empty = !g;
  ASSERT_TRUE( f_is_empty );
  ASSERT_FALSE( g_is_empty );
}

TEST(LinearOperator,Cast)
{
  LinearOperator f = Mock::LinearOperator();

  ASSERT_TRUE( f.target<Mock::LinearOperator>() != nullptr );
}

TEST(LinearOperator,StoreRValue)
{
  LinearOperator f = Mock::LinearOperator(1);
  test(f,1);
}

TEST(LinearOperator,Copy)
{
  LinearOperator g = Mock::LinearOperator(1);
  LinearOperator f = g;
  test(f,1);
}

TEST(LinearOperator,StoreCopy)
{
  auto g = Mock::LinearOperator(1);
  LinearOperator f = g;
  test(f,1);
}

TEST(LinearOperator,Add)
{
  LinearOperator f = Mock::LinearOperator(1), g = Mock::LinearOperator(2);
  f += g;
  test(f,3);
}

TEST(LinearOperator,Subtract)
{
  LinearOperator f = Mock::LinearOperator(1), g = Mock::LinearOperator(2);
  f -= g;
  test(f,-1);
}

TEST(LinearOperator,Multiply)
{
  LinearOperator f = Mock::LinearOperator(1);
  f *= 2;
  test(f,2);
}

TEST(LinearOperator,Negate)
{
  LinearOperator g = Mock::LinearOperator(1);
  auto f = -g;
  test(f,-1);
}

TEST(LinearOperator,Move)
{
  LinearOperator g = Mock::LinearOperator(1);
  bool is_empty_before_move = !g;
  LinearOperator f = std::move(g);
  bool is_empty_after_move = !g;

  EXPECT_FALSE(is_empty_before_move);
  EXPECT_TRUE(is_empty_after_move);

  test(f,1);
}


