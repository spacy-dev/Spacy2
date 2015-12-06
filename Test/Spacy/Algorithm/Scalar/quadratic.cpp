// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include <gtest/gtest.h>

#include "Spacy/Algorithm/Scalar/quadratic.hh"

TEST(Algorithm_Scalar_Quadratic,CreateAndApply)
{
  auto f = Spacy::Quadratic(3,2,1);

  ASSERT_DOUBLE_EQ( f(2) , 11 );
}

