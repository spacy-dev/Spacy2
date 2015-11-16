#include <gtest/gtest.h>

#include "Spacy/Util/Mixins/impl.hh"
#include "Spacy/Util/Base/vectorBase.hh"

#include "Test/mockSetup.hh"

using namespace Spacy;

struct TestDerived : Mixin::Impl<double> , VectorBase , AddArithmeticOperators<TestDerived>
{
  TestDerived(const VectorSpace& space)
    : Mixin::Impl<double>(0.) ,
      VectorBase(space)
  {}

  auto operator()(const TestDerived& y) const
  {
    return Real(impl() * y.impl());
  }
};

TEST(VectorBase,Add)
{
  auto space = createMockBanachSpace();
  TestDerived v0(space), v1(space);
  v0.impl() = 1;
  v1.impl() = 2;

  ASSERT_EQ( (v0 += v1).impl() , 3. );
}

TEST(VectorBase,Subtract)
{
  auto space = createMockBanachSpace();
  TestDerived v0(space), v1(space);
  v0.impl() = 1;
  v1.impl() = 2;

  ASSERT_EQ( (v0 -= v1).impl() , -1. );
}

TEST(VectorBase,Multiply)
{
  auto space = createMockBanachSpace();
  TestDerived v0(space);
  v0.impl() = 2;

  ASSERT_EQ( (v0 *= 3).impl() , 6. );
}

TEST(VectorBase,Negation)
{
  auto space = createMockBanachSpace();
  TestDerived v0(space);
  v0.impl() = 2;

  ASSERT_EQ( (-v0).impl() , -2. );
}

TEST(VectorBase,Comparison)
{
  auto space = createMockBanachSpace();
  TestDerived v0(space), v1(space);
  v0.impl() = 1;
  v1.impl() = v0.impl() - 1e-6;

  ASSERT_TRUE( v0 == v0 );
  ASSERT_FALSE( v0 == v1 );

  space.setEps(1e-6);
  ASSERT_TRUE( v0 == v0 );
  ASSERT_FALSE( v0 == v1 );

  space.setEps(1e-5);
  ASSERT_TRUE( v0 == v0 );
  ASSERT_TRUE( v0 == v1 );

  v0.impl() = 10;
  v1.impl() = v0.impl() - 1e-6;
  space.setEps(1e-6);
  ASSERT_TRUE( v0 == v0 );
  ASSERT_TRUE( v0 == v1 );

  v0.impl() = v1.impl() = 0;
  ASSERT_TRUE( v0 == v1 );
}
