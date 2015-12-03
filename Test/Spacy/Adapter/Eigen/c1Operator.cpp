#include <gtest/gtest.h>

#include "Spacy/Adapter/Eigen/vector.hh"
#include "Spacy/Adapter/Eigen/vectorCreator.hh"
#include "Spacy/Adapter/Eigen/c1Operator.hh"
#include "Spacy/Adapter/Eigen/linearOperator.hh"
#include "setup.hh"

using namespace Spacy;

TEST(Rn,C1Operator_Apply)
{
  auto space = Rn::makeHilbertSpace(testDim());
  auto A = testOperator(space,space);
  auto v = space.zeroVector();
  cast_ref<Rn::Vector>(v) = testVector();
  auto w = A(v);
  const Rn::Vector& w_ = cast_ref<Rn::Vector>(w);
  ASSERT_EQ( w_.get()[0] , 1.1*testVector()[0] );
  ASSERT_EQ( w_.get()[1] , 1.1*testVector()[1] );
}

TEST(Rn,C1Operator_Derivative)
{
  auto space = Rn::makeHilbertSpace(testDim());
  auto A = testOperator(space,space);
  auto v = space.zeroVector(), w = space.zeroVector();
  cast_ref<Rn::Vector>(v) = testVector();
  w = 2*v;
  auto x = A.d1(v,w);
  const Rn::Vector& w_ = cast_ref<Rn::Vector>(w);
  const Rn::Vector& x_ = cast_ref<Rn::Vector>(x);
  ASSERT_EQ( x_.get()[0] , 1.1*w_.get()[0] );
  ASSERT_EQ( x_.get()[1] , 1.1*w_.get()[1] );
}

TEST(Rn,C1Operator_Linearization)
{
  auto space = Rn::makeHilbertSpace(testDim());
  auto A = testOperator(space,space);
  auto v = space.zeroVector();
  cast_ref<Rn::Vector>(v) = testVector();
  auto dA = A.linearization(v);
  auto w = dA(v);
  const Rn::Vector& w_ = cast_ref<Rn::Vector>(w);
  ASSERT_EQ( w_.get()[0] , 1.1*testVector()[0] );
  ASSERT_EQ( w_.get()[1] , 1.1*testVector()[1] );
}
