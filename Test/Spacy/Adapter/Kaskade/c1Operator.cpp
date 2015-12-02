#include <iostream>
#include <gtest/gtest.h>

#include "setup.hh"
#include "Spacy/Adapter/Kaskade/c1Operator.hh"
#include "Spacy/Adapter/Kaskade/vector.hh"
#include "Spacy/Adapter/Kaskade/vectorSpace.hh"

using namespace Kaskade;

template <class Description>
void test(const Spacy::Kaskade::C1Operator<Description>& A, const Spacy::VectorSpace& V)
{
//  auto x0 = A( A.domain().zeroVector() );
//  EXPECT_EQ( norm(x0) , norm(A.domain().zeroVector()) );

  auto x1 = A.d1( A.domain().zeroVector() , A.domain().zeroVector() );
  EXPECT_EQ( norm(x1) , norm(A.domain().zeroVector()) );

  EXPECT_EQ( A.domain().index() , V.index() );
  EXPECT_EQ( A.range().index() , V.index() );
}

void test(const Spacy::C1Operator& A, const Spacy::VectorSpace& V)
{
  auto x0 = A( A.domain().zeroVector() );
//  EXPECT_EQ( norm(x0) , norm(A.domain().zeroVector()) );

  auto x1 = A.d1( A.domain().zeroVector() , A.domain().zeroVector() );
  EXPECT_EQ( norm(x1) , norm(A.domain().zeroVector()) );

  auto dA = A.linearization(A.domain().zeroVector());
  auto x2 = dA(A.domain().zeroVector());
  EXPECT_EQ( norm(x2) , norm(A.domain().zeroVector()) );

  EXPECT_EQ( A.domain().index() , V.index() );
  EXPECT_EQ( A.range().index() , V.index() );
}

TEST(Kaskade,C1Operator_Create)
{
  KASKADE_SINGLE_SPACE_SETUP
  KASKADE_SINGLE_SPACE_OPERATOR

  auto V = Spacy::Kaskade::makeHilbertSpace<Descriptions>(descriptions);
  auto Op = Spacy::Kaskade::makeC1Operator(F,V);

  test(Op,V);
}

TEST(Kaskade,C1Operator_MoveCreateSpacyVector)
{
  KASKADE_SINGLE_SPACE_SETUP
  KASKADE_SINGLE_SPACE_OPERATOR

  auto V = Spacy::Kaskade::makeHilbertSpace<Descriptions>(descriptions);
  Spacy::C1Operator Op = Spacy::Kaskade::makeC1Operator(F,V);

  test(Op,V);
}

TEST(Kaskade,C1Operator_MoveToSpacyVector)
{
  KASKADE_SINGLE_SPACE_SETUP
  KASKADE_SINGLE_SPACE_OPERATOR

  auto V = Spacy::Kaskade::makeHilbertSpace<Descriptions>(descriptions);
  auto Op0 = Spacy::Kaskade::makeC1Operator(F,V);
  Spacy::C1Operator Op = std::move(Op0);

  test(Op,V);
}

TEST(Kaskade,C1Operator_FreeD1)
{
  KASKADE_SINGLE_SPACE_SETUP
  KASKADE_SINGLE_SPACE_OPERATOR

  auto V = Spacy::Kaskade::makeHilbertSpace<Descriptions>(descriptions);
  Spacy::C1Operator Op = Spacy::Kaskade::makeC1Operator(F,V);

  auto lin = d1(Op,Op.domain().zeroVector());
  ASSERT_EQ( norm(lin(Op.domain().zeroVector())) , norm(V.zeroVector()) );
}

//TEST(Rn,C1Operator_Apply)
//{
//  auto space = Rn::makeHilbertSpace(testDim());
//  auto A = testOperator(space,space);
//  auto v = space.zeroVector();
//  cast_ref<Rn::Vector>(v) = testVector();
//  auto w = A(v);
//  const Rn::Vector& w_ = cast_ref<Rn::Vector>(w);
//  ASSERT_EQ( w_.impl()[0] , 1.1*testVector()[0] );
//  ASSERT_EQ( w_.impl()[1] , 1.1*testVector()[1] );
//}

//TEST(Rn,C1Operator_Derivative)
//{
//  auto space = Rn::makeHilbertSpace(testDim());
//  auto A = testOperator(space,space);
//  auto v = space.zeroVector(), w = space.zeroVector();
//  cast_ref<Rn::Vector>(v) = testVector();
//  w = 2*v;
//  auto x = A.d1(v,w);
//  const Rn::Vector& w_ = cast_ref<Rn::Vector>(w);
//  const Rn::Vector& x_ = cast_ref<Rn::Vector>(x);
//  ASSERT_EQ( x_.impl()[0] , 1.1*w_.impl()[0] );
//  ASSERT_EQ( x_.impl()[1] , 1.1*w_.impl()[1] );
//}

//TEST(Rn,C1Operator_Linearization)
//{
//  auto space = Rn::makeHilbertSpace(testDim());
//  auto A = testOperator(space,space);
//  auto v = space.zeroVector();
//  cast_ref<Rn::Vector>(v) = testVector();
//  auto dA = A.linearization(v);
//  auto w = dA(v);
//  const Rn::Vector& w_ = cast_ref<Rn::Vector>(w);
//  ASSERT_EQ( w_.impl()[0] , 1.1*testVector()[0] );
//  ASSERT_EQ( w_.impl()[1] , 1.1*testVector()[1] );
//}
