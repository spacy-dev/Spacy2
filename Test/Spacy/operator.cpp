#include <gtest/gtest.h>

#include "Spacy/operator.hh"
#include "Spacy/Spaces/realSpace.hh"
#include "Test/mockSetup.hh"

using namespace Spacy;

namespace
{
  struct TestOperator
  {
    TestOperator(const VectorSpace& domain, const VectorSpace& range)
      : domain_(&domain), range_(&range)
    {}

    Vector operator()(const Vector&) const
    {
      return Real(3.);
    }

    const VectorSpace& domain() const
    {
      return *domain_;
    }

    const VectorSpace& range() const
    {
      return *range_;
    }

  private:
    const VectorSpace* domain_;
    const VectorSpace* range_;
  };

  void test(const Operator& f, const VectorSpace& X, const VectorSpace& Y)
  {
    EXPECT_DOUBLE_EQ( toDouble(f(X.zeroVector())) , 3 );
    EXPECT_EQ( X.index() , f.domain().index() );
    EXPECT_EQ( Y.index() , f.range().index() );
  }
}

TEST(Operator,IsEmpty)
{
  auto X = createMockBanachSpace();
  auto Y = createMockBanachSpace();
  Operator f;
  Operator g = TestOperator(X,Y);

  bool f_is_empty = !f;
  bool g_is_empty = !g;
  ASSERT_TRUE( f_is_empty );
  ASSERT_FALSE( g_is_empty );
}

TEST(Operator,Cast)
{
  auto X = createMockBanachSpace();
  auto Y = createMockBanachSpace();
  Operator f = TestOperator(X,Y);

  ASSERT_TRUE( f.target<TestOperator>() != nullptr );
}

TEST(Operator,StoreRValue)
{
  auto X = createMockBanachSpace();
  auto Y = createMockBanachSpace();
  Operator f = TestOperator(X,Y);

  test(f,X,Y);
}

TEST(Operator,StoreCopy)
{
  auto X = createMockBanachSpace();
  auto Y = createMockBanachSpace();
  auto g = TestOperator(X,Y);
  Operator f = g;

  test(f,X,Y);
}

TEST(Operator,Copy)
{
  auto X = createMockBanachSpace();
  auto Y = createMockBanachSpace();
  Operator g = TestOperator(X,Y);
  Operator f = g;

  test(f,X,Y);
}

TEST(Operator,Move)
{
  auto X = createMockBanachSpace();
  auto Y = createMockBanachSpace();
  Operator g = TestOperator(X,Y);
  bool is_empty_before_move = !g;
  Operator f = std::move(g);
  bool is_empty_after_move = !g;

  EXPECT_FALSE(is_empty_before_move);
  EXPECT_TRUE(is_empty_after_move);

  test(f,X,Y);
}


