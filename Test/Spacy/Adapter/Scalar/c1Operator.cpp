#include <gtest/gtest.h>

#include <Spacy/Spaces/RealSpace/real.hh>
#include <Spacy/vector.hh>
#include <Spacy/Adapter/Scalar/c1Operator.hh>
#include <Spacy/Adapter/Scalar/linearOperator.hh>

using namespace Spacy;

auto linearFunctionValue(double x)
{
  return 2*x;
}

auto linearFunctionDerivative(double)
{
  return 2;
}

auto quadraticFunctionValue(double x)
{
  return x*x-1;
}

auto quadraticFunctionDerivative(double x)
{
  return 2*x;
}

TEST(ScalarAdapter_C1Operator,LinearExample_Apply)
{
  auto A = Scalar::C1Operator(&linearFunctionValue,&linearFunctionDerivative);
  EXPECT_EQ( toDouble( A(Real(3)) ) , 6. );
}

TEST(ScalarAdapter_C1Operator,LinearExample_Derivative)
{
  auto A = Scalar::C1Operator(&linearFunctionValue,&linearFunctionDerivative);
  EXPECT_EQ( toDouble( A.d1(Real(3),Real(2)) ) , 4. );
}

TEST(ScalarAdapter_C1Operator,LinearExample_Linearization)
{
  auto A = Scalar::C1Operator(&linearFunctionValue,&linearFunctionDerivative);
  auto dA = A.linearization(Real(3));
  EXPECT_EQ( toDouble(dA(Real(2))) , 4. );
  EXPECT_EQ( toDouble(dA.solver()(Real(3))) , 1.5 );
}

TEST(ScalarAdapter_C1Operator,QuadraticExample_Apply)
{
  auto A = Scalar::C1Operator(&quadraticFunctionValue,&quadraticFunctionDerivative);
  EXPECT_EQ( toDouble( A(Real(3)) ) , 8. );
}

TEST(ScalarAdapter_C1Operator,QuadraticExample_Derivative)
{
  auto A = Scalar::C1Operator(&quadraticFunctionValue,&quadraticFunctionDerivative);
  EXPECT_EQ( toDouble( A.d1(Real(3),Real(2)) ) , 12. );
}

TEST(ScalarAdapter_C1Operator,QuadraticExample_Linearization)
{
  auto A = Scalar::C1Operator(&quadraticFunctionValue,&quadraticFunctionDerivative);
  auto dA = A.linearization(Real(3));
  EXPECT_EQ( toDouble(dA(Real(2))) , 12. );
  EXPECT_EQ( toDouble(dA.solver()(Real(3))) , .5 );
}
