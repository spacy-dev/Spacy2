#include <Test/gtest.hh>

#include <Spacy/Algorithm/Generic/step.hh>
#include <Spacy/Util/cast.hh>
#include <Spacy/Util/mixins.hh>

using namespace Spacy;

namespace
{
  struct MixinStep : Mixin::IterativeRefinements , Mixin::Verbosity , Mixin::Eps
  {};
}

TEST(GenericStep,Empty)
{
  auto step = Generic::Step<>();
  EXPECT_EQ( get(cast_ref<Real>(step.getFinalIterate())) , 0 );
}

TEST(GenericStep,Parameters)
{
  auto step = Generic::Step<MixinStep>();
  step.setIterativeRefinements(1);
  EXPECT_EQ( step.getIterativeRefinements() , 1 );
  step.setVerbosityLevel(2);
  EXPECT_EQ( step.getVerbosityLevel() , 2 );
  step.setEps( 1e-3 );
  EXPECT_EQ( step.eps() , 1e-3 );
}
