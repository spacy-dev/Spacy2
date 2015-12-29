#include <gtest/gtest.h>

#include "Spacy/Adapter/Eigen/vector.hh"
#include "Spacy/Adapter/Eigen/vectorCreator.hh"
#include "Spacy/Adapter/Eigen/c2Functional.hh"
#include "Spacy/Adapter/Eigen/linearOperator.hh"
#include "Spacy/Adapter/Eigen/linearSolver.hh"
#include "Spacy/linearSolver.hh"
#include "setup.hh"

using namespace Spacy;

TEST(Rn,C2Functional_Apply)
{
  auto V = Rn::makeHilbertSpace(testDim());
  auto f = testFunctional(V);
  auto v = V.zeroVector();
  cast_ref<Rn::Vector>(v) = testVector();
  auto w = f(v);
  ASSERT_EQ( w , 5. );
}

TEST(Rn,C2Functional_Derivative)
{
  auto V = Rn::makeHilbertSpace(testDim());
  auto f = testFunctional(V);
  auto v = V.zeroVector();
  cast_ref<Rn::Vector>(v) = testVector();
  auto w = f.d1(v)(v);
  ASSERT_EQ( w , 10. );
}

TEST(Rn,C2Functional_SecondDerivative)
{
  auto V = Rn::makeHilbertSpace(testDim());
  auto f = testFunctional(V);
  auto v = V.zeroVector();
  cast_ref<Rn::Vector>(v) = testVector();
  auto w = f.d2(v,v)(v);
  ASSERT_EQ( w , 10. );
}

TEST(Rn,C2Functional_ApplyHessian)
{
  auto V = Rn::makeHilbertSpace(testDim());
  auto f = testFunctional(V);
  auto v = V.zeroVector();
  cast_ref<Rn::Vector>(v) = testVector();
  auto w = f.hessian(v)(v)(v);
  ASSERT_EQ( w , 10. );
}

TEST(Rn,C2Functional_InverseHessian)
{
  auto V = Rn::makeHilbertSpace(testDim());
  auto f = testFunctional(V);
  auto v = V.zeroVector();
  cast_ref<Rn::Vector>(v) = testVector();
  auto A = f.hessian(v).solver();
  auto w = A(v)(v);
  ASSERT_EQ( w , 2.5 );
}
