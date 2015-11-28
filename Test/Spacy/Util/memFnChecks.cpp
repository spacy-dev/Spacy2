// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.


#include <gtest/gtest.h>

#include "Spacy/Util/memFnChecks.hh"

namespace Spacy
{
  class LinearOperator;
  class Vector;
  class VectorSpace;
}

using namespace Spacy;

namespace
{
  struct Empty{};

  struct Domain
  {
    const VectorSpace& domain() const;
  };

  struct Range
  {
    const VectorSpace& range() const;
  };

  struct Space
  {
    const VectorSpace* space() const;
  };

  struct Linearization
  {
    const LinearOperator linearization(const Vector&) const;
  };

  struct Hessian
  {
    const LinearOperator hessian(const Vector&) const;
  };
}

TEST(MemFnChecks,domain)
{
  bool domain = HasMemFn_domain<Domain>::value;
  bool empty = HasMemFn_domain<Empty>::value;
  EXPECT_TRUE(domain);
  EXPECT_FALSE(empty);
}

TEST(MemFnChecks,range)
{
  bool range = HasMemFn_range<Range>::value;
  bool empty = HasMemFn_range<Empty>::value;
  EXPECT_TRUE(range);
  EXPECT_FALSE(empty);
}

TEST(MemFnChecks,space)
{
  bool space = HasMemFn_space<Space>::value;
  bool empty = HasMemFn_space<Empty>::value;
  EXPECT_TRUE(space);
  EXPECT_FALSE(empty);
}

TEST(MemFnChecks,linearization)
{
  bool linearization = HasMemFn_linearization<Linearization,Vector>::value;
  bool empty = HasMemFn_linearization<Empty,Vector>::value;
  EXPECT_TRUE(linearization);
  EXPECT_FALSE(empty);
}

TEST(MemFnChecks,hessian)
{
  bool hessian = HasMemFn_hessian<Hessian,Vector>::value;
  bool empty = HasMemFn_hessian<Empty,Vector>::value;
  EXPECT_TRUE(hessian);
  EXPECT_FALSE(empty);
}
