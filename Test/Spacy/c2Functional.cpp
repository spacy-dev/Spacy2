#include <Test/gtest.hh>

#include <Spacy/c2Functional.hh>
#include <Spacy/linearOperator.hh>
#include <Spacy/operator.hh>
#include <Spacy/Spaces/realSpace.hh>
#include <Test/mockSetup.hh>
#include <Test/Mock/linearOperator.hh>
#include <Test/Mock/c2Functional.hh>

using namespace Spacy;

namespace
{
  void test(const C2Functional&f, const VectorSpace& X)
  {
    EXPECT_DOUBLE_EQ( toDouble(f(zero(X))) , 3 );
    EXPECT_DOUBLE_EQ( toDouble(f.d1(zero(X))) , 2 );
    EXPECT_DOUBLE_EQ( toDouble(f.d2(zero(X),zero(X))) , 1 );
    EXPECT_EQ( X.index() , f.domain().index() );
  }
}


TEST(C2Functional,Assert)
{
  auto X = createMockBanachSpace();
  C2Functional f;
  ASSERT_DEATH( f(zero(X)) , "" );
  ASSERT_DEATH( f.d1(zero(X)) , "" );
  ASSERT_DEATH( f.d2(zero(X),zero(X)) , "" );
  ASSERT_DEATH( f.hessian(zero(X)) , "" );
  ASSERT_DEATH( f.domain() , "" );
}

TEST(C2Functional,IsEmpty)
{
  auto X = createMockBanachSpace();
  C2Functional f;
  C2Functional g = Mock::C2Functional(X);

  bool f_is_empty = !f;
  bool g_is_empty = !g;
  ASSERT_TRUE( f_is_empty );
  ASSERT_FALSE( g_is_empty );
}

TEST(C2Functional,StoreRValue)
{
  auto X = createMockBanachSpace();
  C2Functional f = Mock::C2Functional(X);

  test(f,X);
}

TEST(C2Functional,StoreCopy)
{
  auto X = createMockBanachSpace();
  auto g = Mock::C2Functional(X);
  C2Functional f = g;

  test(f,X);
}

TEST(C2Functional,Copy)
{
  auto X = createMockBanachSpace();
  C2Functional g = Mock::C2Functional(X);
  C2Functional f = g;

  test(f,X);
}

TEST(C2Functional,Move)
{
  auto X = createMockBanachSpace();
  C2Functional g = Mock::C2Functional(X);
  bool is_empty_before_move = !g;
  C2Functional f = std::move(g);
  bool is_empty_after_move = !g;

  EXPECT_FALSE(is_empty_before_move);
  EXPECT_TRUE(is_empty_after_move);

  test(f,X);
}

TEST(C2Functional,AccessHessian)
{
  auto X = createMockBanachSpace();
  C2Functional f = Mock::C2Functional(X);

  auto h = f.hessian(zero(X));
  auto value = is<Mock::LinearOperator>(h);
  ASSERT_TRUE(value);
}
