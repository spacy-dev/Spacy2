#pragma once

#include <gtest/gtest.h>

#include <Spacy/c1Operator.hh>
#include <Spacy/Adapter/scalar.hh>
#include <Spacy/Spaces/RealSpace/real.hh>
#include <Spacy/Util/cast.hh>
#include <Spacy/Util/Exceptions/notConvergedException.hh>

struct Linear
{
  auto relativeAccuracy()
  {
    return 1e-15;
  }

  auto eps()
  {
    return 1e-15;
  }

  auto initialValue()
  {
    return Spacy::Real(0);
  }

  auto example()
  {
    auto value = [](double x){ return 2*x-4; };
    auto derivative = [](double){ return 2.; };
    return Spacy::Scalar::C1Operator(value,derivative);
  }

  auto solution()
  {
    return 2.;
  }
};

struct Quadratic
{
  auto relativeAccuracy()
  {
    return 1e-15;
  }

  auto eps()
  {
    return 1e-15;
  }

  auto initialValue()
  {
    return Spacy::Real(5);
  }

  auto example()
  {
    auto value = [](double x){ return x*x-4; };
    auto derivative = [](double x){ return 2*x; };
    return Spacy::Scalar::C1Operator(value,derivative);
  }

  auto solution()
  {
    return 2.;
  }
};

#define GENERATE_SCALAR_TEST(ALGORITHM,TESTCASE,REQUIRED_STEPS) \
TEST(EquationSolver_##ALGORITHM##_Test,TESTCASE##Equation) \
{ \
  TESTCASE testCase; \
  auto solver1 = createSolver(testCase.example(),testCase.relativeAccuracy(),testCase.eps(),REQUIRED_STEPS); \
  auto solver2 = createSolver(testCase.example(),testCase.relativeAccuracy(),testCase.eps(),REQUIRED_STEPS+1); \
  auto solver3 = createSolver(testCase.example(),testCase.relativeAccuracy(),testCase.eps(),REQUIRED_STEPS-1); \
  \
  auto x0 = testCase.initialValue(); \
  auto x1 = solver1(x0); \
  auto x2 = solver2(x0); \
  \
  ASSERT_TRUE( x1==x2 ); \
  ASSERT_EQ( get(cast_ref<Spacy::Real>(x1)) , testCase.solution() ); \
  ASSERT_EQ( get(cast_ref<Spacy::Real>(x2)) , testCase.solution() ); \
  ASSERT_THROW( solver3(x0) , Spacy::Exception::NotConverged ); \
}
