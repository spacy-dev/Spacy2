// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include <gtest/gtest.h>

#include "Spacy/Algorithm/Scalar/cubic.hh"

TEST(Algorithm_Scalar_Cubic,CreateAndApply)
{
  auto f = Spacy::Cubic(4,3,2,1);

  ASSERT_DOUBLE_EQ( toDouble(f(2)) , 26 );
}


