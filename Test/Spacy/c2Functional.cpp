#include <gtest/gtest.h>

#include "Spacy/c2Functional.hh"
#include "Spacy/operator.hh"
#include "Spacy/Spaces/realSpace.hh"
#include "Test/mockSetup.hh"
#include "Test/Mock/linearOperator.hh"

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

    Vector d2(const Vector&, const Vector&) const
    {
      return Real(1.);
    }

    LinearOperator hessian(const ::Spacy::Vector&) const
    {
      return Mock::LinearOperator();
    }

    const VectorSpace& domain() const
    {
      return *domain_;
    }

  private:
    const VectorSpace* domain_;
  };

  void test(const C2Functional&f, const VectorSpace& X)
  {
    EXPECT_DOUBLE_EQ( toDouble(f(X.zeroVector())) , 3 );
    EXPECT_DOUBLE_EQ( toDouble(f.d1(X.zeroVector())) , 2 );
    EXPECT_DOUBLE_EQ( toDouble(f.d2(X.zeroVector(),X.zeroVector())) , 1 );
    EXPECT_EQ( X.index() , f.domain().index() );
  }
}

TEST(C2Functional,IsEmpty)
{
  auto X = createMockBanachSpace();
  C2Functional f;
  C2Functional g = TestFunctional(X);

  bool f_is_empty = !f;
  bool g_is_empty = !g;
  ASSERT_TRUE( f_is_empty );
  ASSERT_FALSE( g_is_empty );
}

TEST(C2Functional,StoreRValue)
{
  auto X = createMockBanachSpace();
  C2Functional f = TestFunctional(X);

  test(f,X);
}

TEST(C2Functional,StoreCopy)
{
  auto X = createMockBanachSpace();
  auto g = TestFunctional(X);
  C2Functional f = g;

  test(f,X);
}

TEST(C2Functional,Copy)
{
  auto X = createMockBanachSpace();
  C2Functional g = TestFunctional(X);
  C2Functional f = g;

  test(f,X);
}

TEST(C2Functional,Move)
{
  auto X = createMockBanachSpace();
  C2Functional g = TestFunctional(X);
  bool is_empty_before_move = !g;
  C2Functional f = std::move(g);
  bool is_empty_after_move = !g;

  EXPECT_FALSE(is_empty_before_move);
  EXPECT_TRUE(is_empty_after_move);

  test(f,X);
}


