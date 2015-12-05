// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include <gtest/gtest.h>

#include "Spacy/Util/invoke.hh"
#include "Spacy/Algorithm/Scalar/quadratic.hh"

namespace
{
  auto makeQuadratic(double a, double b, double c)
  {
    return Spacy::Quadratic(a,b,c);
  }
}

TEST(Invoke,MakeQuadratic)
{
  auto args = std::make_tuple(3,2,1);
  auto f = Spacy::invoke(makeQuadratic,args);

  ASSERT_DOUBLE_EQ( toDouble(f(2)) , 11 );
}

TEST(Create,CreateQuadratic)
{
  auto args = std::make_tuple(3,2,1);
  auto f = Spacy::create<Spacy::Quadratic>(args);

  ASSERT_DOUBLE_EQ( toDouble(f(2)) , 11 );
}
