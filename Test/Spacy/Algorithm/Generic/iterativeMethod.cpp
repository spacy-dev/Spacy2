#include <Test/gtest.hh>

#include <Spacy/vector.hh>
#include <Spacy/Spaces/ScalarSpace/Real.hh>
#include <Spacy/Algorithm/Generic/iterativeMethod.hh>
#include <Spacy/Util/cast.hh>
#include <Spacy/Util/mixins.hh>

#include <string>

using namespace Spacy;

namespace
{
  struct TerminationCriterion
  {
    TerminationCriterion(bool val=true)
      : value(val)
    {}

    void init()
    {
      initialized = true;
    }

    operator bool() const
    {
      return value;
    }

    template <class Step>
    void connect(const Step&){}

    bool initialized = false;
    bool value = true;
  };

  struct MixinTerminationCriterion
      : Mixin::AbsoluteAccuracy,
        Mixin::RelativeAccuracy,
        Mixin::MinimalAccuracy,
        Mixin::Eps,
        Mixin::Verbosity
  {
    void init()
    {
      initialized = true;
    }

    operator bool() const
    {
      return true;
    }

    template <class Step>
    void connect(const Step&){}

    bool initialized = false;
  };

  struct Step
  {
    void init(Vector& x, Vector& b)
    {
      x0 = x;
      b0 = b;
      initialized = true;
    }

    void reset(Vector& x, Vector& b)
    {
      x0 = x;
      b0 = b;
      resetted = true;
    }

    void compute()
    {
      x0 += b0;
    }

    Vector getFinalIterate()
    {
      postProcessed = true;
      return x0;
    }

    std::string name() const
    {
      return "dummy step";
    }

    bool initialized = false, resetted = false, postProcessed = false;
    Vector x0, b0;
  };

  struct RestartingStep
  {
    RestartingStep(bool restart = false)
      : doRestart(restart)
    {}

    void init(Vector& x, Vector& b)
    {
      x0 = x;
      b0 = b;
      initialized = true;
    }

    void reset(Vector& x, Vector& b)
    {
      x0 = x;
      b0 = b;
      resetted = true;
    }

    void compute()
    {
      x0 += b0;
    }

    bool terminate() const
    {
      return doTerminate;
    }

    bool restart() const
    {
      doTerminate = true;
      return doRestart;
    }

    Vector getFinalIterate()
    {
      postProcessed = true;
      return x0;
    }

    std::string name() const
    {
      return "dummy step";
    }

    bool initialized = false, resetted = false, postProcessed = false, doRestart = false;
    mutable bool doTerminate = false;
    Vector x0, b0;
  };

  struct TerminatingStep
  {
    void init(Vector& x, Vector& b)
    {
      x0 = x;
      b0 = b;
      initialized = true;
    }

    void reset(Vector& x, Vector& b)
    {
      x0 = x;
      b0 = b;
      resetted = true;
    }

    void compute()
    {
      x0 += b0;
    }

    Vector getFinalIterate()
    {
      postProcessed = true;
      return x0;
    }

    std::string name() const
    {
      return "dummy step";
    }

    bool terminate() const
    {
      return doTerminate;
    }

    bool initialized = false, resetted = false, postProcessed = false, doTerminate=false;
    Vector x0, b0;
  };

  inline Real testAccuracy()
  {
    return 1e-6;
  }
}


TEST(GenericIterativeMethod,NotConvergedInZeroSteps)
{
  auto dummySolver = Generic::IterativeMethod<Step,TerminationCriterion>(Step(),TerminationCriterion());
  dummySolver.setMaxSteps(0);
  Vector x = Real(0), b = Real(1);

  EXPECT_THROW( dummySolver(x,b) , Exception::NotConverged );
  EXPECT_TRUE( dummySolver.initialized );
  EXPECT_TRUE( dummySolver.getTerminationCriterion().initialized);
  EXPECT_FALSE( dummySolver.resetted );
  EXPECT_FALSE( dummySolver.postProcessed );
}

TEST(GenericIterativeMethod,NotConverged)
{
  auto dummySolver = Generic::IterativeMethod<Step,TerminationCriterion>(Step(),TerminationCriterion(false));
  dummySolver.setMaxSteps(0);
  Vector x = Real(0), b = Real(1);

  EXPECT_THROW( dummySolver(x,b) , Exception::NotConverged );
  EXPECT_TRUE( dummySolver.initialized );
  EXPECT_TRUE( dummySolver.getTerminationCriterion().initialized);
  EXPECT_FALSE( dummySolver.resetted );
  EXPECT_FALSE( dummySolver.postProcessed );
}

TEST(GenericIterativeMethod,NotConverged_TerminatingStep)
{
  TerminatingStep step;
  auto dummySolver = Generic::IterativeMethod<TerminatingStep,TerminationCriterion>(step,TerminationCriterion(false));
  dummySolver.setMaxSteps(0);
  Vector x = Real(0), b = Real(1);

  EXPECT_THROW( dummySolver(x,b) , Exception::NotConverged );
  EXPECT_TRUE( dummySolver.initialized );
  EXPECT_TRUE( dummySolver.getTerminationCriterion().initialized);
  EXPECT_FALSE( dummySolver.resetted );
  EXPECT_FALSE( dummySolver.postProcessed );
}

TEST(GenericIterativeMethod,Converged)
{
  auto dummySolver = Generic::IterativeMethod<Step,TerminationCriterion>(Step(),TerminationCriterion());
  dummySolver.setMaxSteps(1);
  Vector x = Real(0), b = Real(1);

  x = dummySolver(x,b);
  EXPECT_TRUE( dummySolver.initialized );
  EXPECT_FALSE( dummySolver.resetted );
  EXPECT_TRUE( dummySolver.postProcessed );
  EXPECT_DOUBLE_EQ( get(cast_ref<Real>(x)) , 1 );
}

TEST(GenericIterativeMethod,Converged_TerminatingStep)
{
  TerminatingStep step;
  step.doTerminate = true;
  auto dummySolver = Generic::IterativeMethod<TerminatingStep,TerminationCriterion>(step,TerminationCriterion(false));
  dummySolver.setMaxSteps(1);
  Vector x = Real(0), b = Real(1);

  x = dummySolver(x,b);
  EXPECT_TRUE( dummySolver.initialized );
  EXPECT_FALSE( dummySolver.resetted );
  EXPECT_TRUE( dummySolver.postProcessed );
  EXPECT_DOUBLE_EQ( get(cast_ref<Real>(x)) , 1 );
}

TEST(GenericIterativeMethod,RestartAndTerminate)
{
  RestartingStep step(true);
  auto dummySolver = Generic::makeIterativeMethod(step,TerminationCriterion(false));
  dummySolver.setMaxSteps(1);
  Vector x = Real(0), b = Real(1);

  x = dummySolver(x,b);
  EXPECT_TRUE( dummySolver.initialized );
  EXPECT_TRUE( dummySolver.resetted );
  EXPECT_TRUE( dummySolver.postProcessed );
  EXPECT_DOUBLE_EQ( get(cast_ref<Real>(x)) , 1 );
}

TEST(GenericIterativeMethod,RestartAndDontTerminate)
{
  RestartingStep step(true);
  auto dummySolver = Generic::makeIterativeMethod(step,TerminationCriterion(false));
  dummySolver.setMaxSteps(1);
  Vector x = Real(0), b = Real(1);

  x = dummySolver(x,b);
  EXPECT_TRUE( dummySolver.initialized );
  EXPECT_TRUE( dummySolver.resetted );
  EXPECT_TRUE( dummySolver.postProcessed );
  EXPECT_DOUBLE_EQ( get(cast_ref<Real>(x)) , 1 );
}

TEST(GenericIterativeMethod,MixinParameters)
{
  auto dummySolver = Generic::IterativeMethod<Step,MixinTerminationCriterion>(Step(),MixinTerminationCriterion());
  dummySolver.setAbsoluteAccuracy(testAccuracy());
  EXPECT_EQ( dummySolver.getTerminationCriterion().getAbsoluteAccuracy() , testAccuracy() );
  dummySolver.setRelativeAccuracy(testAccuracy());
  EXPECT_EQ( dummySolver.getTerminationCriterion().getRelativeAccuracy() , testAccuracy() );
  dummySolver.setMinimalAccuracy(testAccuracy());
  EXPECT_EQ( dummySolver.getTerminationCriterion().getMinimalAccuracy() , testAccuracy() );
  dummySolver.set_eps(testAccuracy());
  EXPECT_EQ( dummySolver.getTerminationCriterion().eps() , testAccuracy() );
  dummySolver.setVerbosityLevel(2);
  EXPECT_EQ( dummySolver.getTerminationCriterion().getVerbosityLevel() , 2 );
}
