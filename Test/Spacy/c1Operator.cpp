#include <gtest/gtest.h>

#include "Spacy/operator.hh"
#include "Spacy/Spaces/realSpace.hh"
#include "Test/mockSetup.hh"
#include "Test/Mock/linearOperator.hh"

using namespace Spacy;

namespace
{
  struct TestC1Operator
  {
    TestC1Operator(const VectorSpace& domain, const VectorSpace& range)
      : domain_(domain), range_(range)
    {}

    Vector operator()(const Vector&) const
    {
      return Real(3.);
    }

    Vector d1(const Vector&,const Vector&) const
    {
      return Real(2.);
    }

    LinearOperator linearization(const Vector&) const
    {
      return Mock::LinearOperator();
    }

    const VectorSpace& domain() const
    {
      return domain_;
    }

    const VectorSpace& range() const
    {
      return range_;
    }

  private:
    const VectorSpace& domain_;
    const VectorSpace& range_;
  };

  void test(const C1Operator& f, const VectorSpace& X, const VectorSpace& Y)
  {
    EXPECT_EQ( toDouble(f(X.zeroVector())) , 3. );
    EXPECT_EQ( toDouble(f.d1(X.zeroVector(),X.zeroVector())) , 2. );
    EXPECT_EQ( X.index() , f.domain().index() );
    EXPECT_EQ( Y.index() , f.range().index() );

    auto L = f.linearization(X.zeroVector());
    auto expected = is<Mock::LinearOperator>(L);
    EXPECT_TRUE(expected);
  }
}

TEST(C1Operator,IsEmpty)
{
  auto X = createMockBanachSpace();
  auto Y = createMockBanachSpace();
  C1Operator f;
  C1Operator g = TestC1Operator(X,Y);

  bool f_is_empty = !f;
  bool g_is_empty = !g;
  ASSERT_TRUE( f_is_empty );
  ASSERT_FALSE( g_is_empty );
}

//TEST(C1Operator,Cast)
//{
//  auto X = createMockBanachSpace();
//  auto Y = createMockBanachSpace();
//  C1Operator f = TestC1Operator(X,Y);

//  ASSERT_TRUE( f.target<TestC1Operator>() != nullptr );
//}

TEST(C1Operator,StoreRValue)
{
  auto X = createMockBanachSpace();
  auto Y = createMockBanachSpace();
  C1Operator f = TestC1Operator(X,Y);

  test(f,X,Y);
}

TEST(C1Operator,StoreCopy)
{
  auto X = createMockBanachSpace();
  auto Y = createMockBanachSpace();
  auto g = TestC1Operator(X,Y);
  C1Operator f = g;

  test(f,X,Y);
}

TEST(C1Operator,Copy)
{
  auto X = createMockBanachSpace();
  auto Y = createMockBanachSpace();
  C1Operator g = TestC1Operator(X,Y);
  C1Operator f = g;

  test(f,X,Y);
  test(g,X,Y);
}

TEST(C1Operator,Move)
{
  auto X = createMockBanachSpace();
  auto Y = createMockBanachSpace();
  C1Operator g = TestC1Operator(X,Y);
  bool is_empty_before_move = !g;
  C1Operator f = std::move(g);
  bool is_empty_after_move = !g;

  EXPECT_FALSE(is_empty_before_move);
  EXPECT_TRUE(is_empty_after_move);

  test(f,X,Y);
}


