#include <gtest/gtest.h>

#include <Spacy/vector.hh>
#include <Spacy/linearOperator.hh>
#include <Spacy/Spaces/realSpace.hh>
#include <Spacy/Util/Mixins/target.hh>
#include <Test/mockSetup.hh>
#include <Test/Mock/linearOperator.hh>
#include <Test/Mock/linearSolver.hh>

using namespace Spacy;

namespace
{
  void test(const LinearOperator& f, double value)
  {
    EXPECT_DOUBLE_EQ( toDouble(f(zero(Space::R))) , value );
    EXPECT_EQ( 0u , f.domain().index() );
    EXPECT_EQ( 0u , f.range().index() );

    auto L = f.solver();
    auto expected = Spacy::target<Mock::IndefiniteLinearSolver>(L) != nullptr;
    EXPECT_TRUE(expected);
  }
}

TEST(LinearOperator,Assert)
{
  auto X = createMockBanachSpace();
  LinearOperator f;
  ASSERT_DEATH( f( zero(X) ) , "" );
  ASSERT_DEATH( f.solver() , "" );
  ASSERT_DEATH( f.domain() , "" );
  ASSERT_DEATH( f.range() , "" );
  ASSERT_DEATH( -f , "" );

  LinearOperator g;
  ASSERT_DEATH( f*=1 , "" );
  ASSERT_DEATH( f+=g , "" );
  ASSERT_DEATH( f-=g , "" );
  ASSERT_DEATH( f(g) , "" );

  f = Mock::LinearOperator(1);
  ASSERT_DEATH( f+=g , "" );
  ASSERT_DEATH( f-=g , "" );
  ASSERT_DEATH( f(g) , "" );

  ASSERT_DEATH( g+=f , "" );
  ASSERT_DEATH( g-=f , "" );
  ASSERT_DEATH( g(f) , "" );
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


