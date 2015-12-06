// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include <gtest/gtest.h>

#include "Spacy/Algorithm/Generic/step.hh"
#include "Spacy/Util/mixins.hh"

using namespace Spacy;

namespace
{
  struct MixinStep : Mixin::IterativeRefinements , Mixin::Verbosity , Mixin::Eps
  {};
}

TEST(GenericStep,Empty)
{
  auto step = Generic::Step<>();
  ASSERT_DOUBLE_EQ( toDouble(step.getFinalIterate()) , 0 );
}

TEST(GenericStep,Parameters)
{
  auto step = Generic::Step<MixinStep>();
  step.setIterativeRefinements(1);
  ASSERT_EQ( step.getIterativeRefinements() , 1 );
  step.setVerbosityLevel(2);
  ASSERT_EQ( step.getVerbosityLevel() , 2 );
  step.setEps( 1e-3 );
  ASSERT_DOUBLE_EQ( step.eps() , 1e-3 );
}
