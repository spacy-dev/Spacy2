#include <Test/gtest.hh>

#include <Spacy/Spaces/ScalarSpace/Real.hh>

#include <Test/mockSetup.hh>

using namespace Spacy;

TEST(VectorBase,Add)
{
  auto space = createMockBanachSpace();
  Real v0(space), v1(space);
  v0.get() = 1;
  v1.get() = 2;

  ASSERT_EQ( (v0 += v1).get() , 3. );
}

TEST(VectorBase,Subtract)
{
  auto space = createMockBanachSpace();
  Real v0(space), v1(space);
  v0.get() = 1;
  v1.get() = 2;

  ASSERT_EQ( (v0 -= v1).get() , -1. );
}

TEST(VectorBase,Multiply)
{
  auto space = createMockBanachSpace();
  Real v0(space);
  v0.get() = 2;

  ASSERT_EQ( (v0 *= 3).get() , 6. );
}

TEST(VectorBase,Negation)
{
  auto space = createMockBanachSpace();
  Real v0(space);
  v0.get() = 2;

  ASSERT_EQ( (-v0).get() , -2. );
}

TEST(VectorBase,Comparison)
{
  auto space = createMockBanachSpace();
  Real v0(space), v1(space);
  v0.get() = 1;
  v1.get() = v0.get() - 1e-6;

  ASSERT_TRUE( v0 == v0 );
  ASSERT_FALSE( v0 == v1 );

  space.set_eps(1e-6);
  ASSERT_TRUE( v0 == v0 );
  ASSERT_FALSE( v0 == v1 );

  space.set_eps(1e-5);
  ASSERT_TRUE( v0 == v0 );
  ASSERT_TRUE( v0 == v1 );

  v0.get() = 10;
  v1.get() = v0.get() - 1e-6;
  space.set_eps(1e-6);
  ASSERT_TRUE( v0 == v0 );
  ASSERT_TRUE( v0 == v1 );

  v0.get() = v1.get() = 0;
  ASSERT_TRUE( v0 == v1 );
}
