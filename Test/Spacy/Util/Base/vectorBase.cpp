// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include <gtest/gtest.h>

#include "Spacy/Util/Mixins/get.hh"
#include "Spacy/Util/Base/addArithmeticOperators.hh"
#include "Spacy/Util/Base/vectorBase.hh"

#include "Test/mockSetup.hh"

using namespace Spacy;

struct TestDerived : Mixin::Get<double> , VectorBase , AddArithmeticOperators<TestDerived>
{
  TestDerived(const VectorSpace& space)
    : Mixin::Get<double>(0.) ,
      VectorBase(space)
  {}

  auto operator()(const TestDerived& y) const
  {
    return Real(get() * y.get());
  }
};

TEST(VectorBase,Add)
{
  auto space = createMockBanachSpace();
  TestDerived v0(space), v1(space);
  v0.get() = 1;
  v1.get() = 2;

  ASSERT_EQ( (v0 += v1).get() , 3. );
}

TEST(VectorBase,Subtract)
{
  auto space = createMockBanachSpace();
  TestDerived v0(space), v1(space);
  v0.get() = 1;
  v1.get() = 2;

  ASSERT_EQ( (v0 -= v1).get() , -1. );
}

TEST(VectorBase,Multiply)
{
  auto space = createMockBanachSpace();
  TestDerived v0(space);
  v0.get() = 2;

  ASSERT_EQ( (v0 *= 3).get() , 6. );
}

TEST(VectorBase,Negation)
{
  auto space = createMockBanachSpace();
  TestDerived v0(space);
  v0.get() = 2;

  ASSERT_EQ( (-v0).get() , -2. );
}

TEST(VectorBase,Comparison)
{
  auto space = createMockBanachSpace();
  TestDerived v0(space), v1(space);
  v0.get() = 1;
  v1.get() = v0.get() - 1e-6;

  ASSERT_TRUE( v0 == v0 );
  ASSERT_FALSE( v0 == v1 );

  space.setEps(1e-6);
  ASSERT_TRUE( v0 == v0 );
  ASSERT_FALSE( v0 == v1 );

  space.setEps(1e-5);
  ASSERT_TRUE( v0 == v0 );
  ASSERT_TRUE( v0 == v1 );

  v0.get() = 10;
  v1.get() = v0.get() - 1e-6;
  space.setEps(1e-6);
  ASSERT_TRUE( v0 == v0 );
  ASSERT_TRUE( v0 == v1 );

  v0.get() = v1.get() = 0;
  ASSERT_TRUE( v0 == v1 );
}
