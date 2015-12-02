#include <gtest/gtest.h>

#include "Spacy/functional.hh"
#include "Spacy/Spaces/realSpace.hh"
#include "Test/mockSetup.hh"

using namespace Spacy;

namespace
{
  struct TestFunctional
  {
    TestFunctional(const VectorSpace& space)
      : domain_(&space)
    {}

    Real operator()(const Vector&) const
    {
      return Real(3.);
    }

    const VectorSpace& domain() const
    {
      return *domain_;
    }

  private:
    const VectorSpace* domain_;
  };

  void test(const Functional& f, const VectorSpace& X)
  {
    EXPECT_EQ( toDouble(f(X.zeroVector())) , 3. );
    EXPECT_EQ( X.index() , f.domain().index() );
  }
}

TEST(Functional,IsEmpty)
{
  auto X = createMockBanachSpace();
  Functional f;
  Functional g = TestFunctional(X);

  bool f_is_empty = !f;
  bool g_is_empty = !g;
  ASSERT_TRUE( f_is_empty );
  ASSERT_FALSE( g_is_empty );
}

TEST(Functional,Cast)
{
  auto X = createMockBanachSpace();
  Functional f = TestFunctional(X);

  ASSERT_TRUE( f.target<TestFunctional>() != nullptr );
}

TEST(Functional,StoreRValue)
{
  auto X = createMockBanachSpace();
  Functional f = TestFunctional(X);

  test(f,X);
}

TEST(Functional,StoreCopy)
{
  auto X = createMockBanachSpace();
  auto g = TestFunctional(X);
  Functional f = g;

  test(f,X);
}

TEST(Functional,Copy)
{
  auto X = createMockBanachSpace();
  Functional g = TestFunctional(X);
  Functional f = g;

  test(f,X);
}

TEST(Functional,Move)
{
  auto X = createMockBanachSpace();
  Functional g = TestFunctional(X);
  bool is_empty_before_move = !g;
  Functional f = std::move(g);
  bool is_empty_after_move = !g;

  EXPECT_FALSE(is_empty_before_move);
  EXPECT_TRUE(is_empty_after_move);

  test(f,X);
}


