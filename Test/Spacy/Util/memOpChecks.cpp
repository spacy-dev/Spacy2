// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include <gtest/gtest.h>

#include "Spacy/Util/memOpChecks.hh"
#include "Spacy/Util/Mixins/get.hh"
#include "Spacy/Util/Base/addArithmeticOperators.hh"
#include "Spacy/Util/Base/vectorBase.hh"

using namespace Spacy;

namespace
{
  struct Empty{};

  struct Negate
  {
    Negate operator-() const;
  };

  struct WrongNegate
  {
    WrongNegate& operator-() const;
  };

  struct Add
  {
    Add& operator+=(const Add&);
  };

  struct WrongAdd
  {
    WrongAdd operator+=(const WrongAdd&);
  };

  struct Subtract
  {
    Subtract& operator-=(const Subtract&);
  };

  struct WrongSubtract
  {
    WrongSubtract operator-=(const WrongSubtract&);
  };

  struct Multiply
  {
    Multiply& operator*=(double);
  };

  struct WrongMultiply
  {
    WrongMultiply operator*=(double);
  };

  struct Callable
  {
    double operator()(double);
  };

  struct WrongCallable
  {
    void operator()(double);
  };

  struct NegateFromVectorBase : Mixin::Get<double> , AddArithmeticOperators<NegateFromVectorBase>
  {};
}

TEST(MemOpChecks,Negate)
{
  bool negate = HasMemOp_negate<Negate>::value;
  bool wrongNegate = HasMemOp_negate<WrongNegate>::value;
  bool negateFromVectorBase = HasMemOp_negate<NegateFromVectorBase>::value;
  bool empty = HasMemOp_negate<Empty>::value;
  EXPECT_TRUE( negate );
  EXPECT_FALSE( wrongNegate );
  EXPECT_TRUE( negateFromVectorBase );
  EXPECT_FALSE( empty );
}

TEST(MemOpChecks,Add)
{
  auto add = HasMemOp_add<Add>::value;
  auto wrongAdd = HasMemOp_add<WrongAdd>::value;
  auto empty = HasMemOp_add<Empty>::value;
  EXPECT_TRUE(add);
  EXPECT_FALSE(wrongAdd);
  EXPECT_FALSE(empty);
}

TEST(MemOpChecks,Subtract)
{
  auto subtract = HasMemOp_subtract<Subtract>::value;
  auto wrongSubtract = HasMemOp_subtract<WrongSubtract>::value;
  auto empty = HasMemOp_subtract<Empty>::value;
  EXPECT_TRUE(subtract);
  EXPECT_FALSE(wrongSubtract);
  EXPECT_FALSE(empty);
}

TEST(MemOpChecks,Multiply)
{
  auto multiply = HasMemOp_multiply<Multiply>::value;
  auto wrongMultiply = HasMemOp_multiply<WrongMultiply>::value;
  auto empty = HasMemOp_multiply<Empty>::value;
  EXPECT_TRUE(multiply);
  EXPECT_FALSE(wrongMultiply);
  EXPECT_FALSE(empty);
}

TEST(MemOpChecks,Callable)
{
  auto callable = HasMemOp_callable<Callable,double,double>::value;
  auto wrongCallable = HasMemOp_callable<WrongCallable,double,double>::value;
  auto empty = HasMemOp_callable<Empty,double,double>::value;
  EXPECT_TRUE(callable);
  EXPECT_FALSE(wrongCallable);
  EXPECT_FALSE(empty);
}


