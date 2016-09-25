#include <Test/gtest.hh>

#include <Spacy/c1Functional.hh>
#include <Spacy/Spaces/realSpace.hh>
#include <Test/mockSetup.hh>

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

    Vector d1(const Vector&) const
    {
      return Real(2.);
    }

    const VectorSpace& domain() const
    {
      return *domain_;
    }

  private:
    const VectorSpace* domain_;
  };

  void test(const C1Functional&f, const VectorSpace& X)
  {
    EXPECT_DOUBLE_EQ( toDouble(f(zero(X))) , 3 );
    EXPECT_DOUBLE_EQ( toDouble(f.d1(zero(X))) , 2 );
    EXPECT_EQ( X.index() , f.domain().index() );
  }
}

TEST(C1Functional,Assert)
{
  auto X = createMockBanachSpace();
  C1Functional f;
  EXPECT_DEATH( f(zero(X)) , "" );
  EXPECT_DEATH( f.d1(zero(X)) , "" );
  EXPECT_DEATH( f.domain() , "" );
}

TEST(C1Functional,IsEmpty)
{
  auto X = createMockBanachSpace();
  C1Functional f;
  C1Functional g = TestFunctional(X);

  bool f_is_empty = !f;
  bool g_is_empty = !g;
  EXPECT_TRUE( f_is_empty );
  EXPECT_FALSE( g_is_empty );
}

TEST(C1Functional,StoreRValue)
{
  auto X = createMockBanachSpace();
  C1Functional f = TestFunctional(X);

  test(f,X);
}

TEST(C1Functional,StoreCopy)
{
  auto X = createMockBanachSpace();
  auto g = TestFunctional(X);
  C1Functional f = g;

  test(f,X);
  test(g,X);
}

TEST(C1Functional,Copy)
{
  auto X = createMockBanachSpace();
  C1Functional g = TestFunctional(X);
  C1Functional f = g;

  test(f,X);
  test(g,X);
}

TEST(C1Functional,Move)
{
  auto X = createMockBanachSpace();
  C1Functional g = TestFunctional(X);
  bool is_empty_before_move = !g;
  C1Functional f = std::move(g);
  bool is_empty_after_move = !g;

  EXPECT_FALSE(is_empty_before_move);
  EXPECT_TRUE(is_empty_after_move);

  test(f,X);
}


