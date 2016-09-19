#include <gtest/gtest.h>

#include "Test/mockSetup.hh"

using namespace Spacy;

TEST(VectorSpace,Index)
{
  auto space1 = createMockBanachSpace();
  auto space2 = createMockBanachSpace();
  ASSERT_EQ( space1.index()+1 , space2.index() );
}

TEST(VectorSpace,Vector)
{
  auto space = createMockBanachSpace();
  auto vector = zero(space);
  ASSERT_EQ( mockValue(vector) , static_cast<double>(Mock::Vector::testValue) );
}

TEST(VectorSpace,Norm)
{
  auto space = createMockBanachSpace();
  auto vector = zero(space);
  ASSERT_EQ( toDouble(space.norm()(vector))  , static_cast<double>(Mock::Norm::testValue) );
}

TEST(VectorSpace,SetNorm)
{
  auto space = createMockBanachSpace();
  auto vector = zero(space);
  space.setNorm(Mock::Norm10());
  ASSERT_EQ( toDouble(space.norm()(vector))  , static_cast<double>(Mock::Norm10::testValue) );
}

TEST(VectorSpace,BanachSpaceScalarProduct)
{
  auto space = createMockBanachSpace();
  ASSERT_ANY_THROW( space.scalarProduct() );
}

TEST(VectorSpace,SetScalarProduct)
{
  auto space = createMockBanachSpace();
  auto vector = zero(space);
  space.setScalarProduct(Mock::ScalarProduct());
  ASSERT_EQ( toDouble(space.scalarProduct()(vector,vector))  , static_cast<double>(Mock::ScalarProduct::testValue) );
}

TEST(VectorSpace,IsHilbertSpaceNot)
{
  auto space = createMockBanachSpace();
  ASSERT_FALSE( space.isHilbertSpace() );
}

TEST(VectorSpace,IsHilbertSpace)
{
  auto space = createMockBanachSpace();
  space.setScalarProduct(Mock::ScalarProduct());
  ASSERT_TRUE( space.isHilbertSpace() );
}

TEST(VectorSpace,MakeBanachSpace)
{
  auto space = createMockBanachSpace();
  auto vector = zero(space);
  ASSERT_EQ( mockValue(vector) , static_cast<double>(Mock::Vector::testValue) );
  ASSERT_EQ( toDouble(space.norm()(vector))  , static_cast<double>(Mock::Norm::testValue) );
  ASSERT_FALSE( space.isHilbertSpace() );
}

TEST(VectorSpace,MakeHilbertSpace)
{
  auto space = makeHilbertSpace( Mock::VectorCreator{} , Mock::ScalarProduct{} );
  auto vector = zero(space);
  ASSERT_EQ( mockValue(vector) , static_cast<double>(Mock::Vector::testValue) );
  ASSERT_EQ( toDouble(space.norm()(vector))  , static_cast<double>(Mock::Norm::testValue) );
  ASSERT_EQ( toDouble(space.scalarProduct()(vector,vector))  , static_cast<double>(Mock::ScalarProduct::testValue) );
  ASSERT_TRUE( space.isHilbertSpace() );
}

TEST(VectorSpace, SetDualSpace)
{
  auto V = makeBanachSpace( Mock::VectorCreator{} , Mock::Norm{} );
  auto W = makeBanachSpace( Mock::VectorCreator{} , Mock::Norm{} );
  V.setDualSpace(&W);
  ASSERT_EQ( W.index() , V.dualSpace().index() );
  ASSERT_TRUE( V.isPrimalWRT(W) );
  ASSERT_FALSE( W.isPrimalWRT(V) );
  ASSERT_THROW( W.dualSpace() , std::runtime_error );
}

TEST(VectorSpace, AddDualSpace)
{
  auto V = makeBanachSpace( Mock::VectorCreator{} , Mock::Norm{} );
  auto W = makeBanachSpace( Mock::VectorCreator{} , Mock::Norm{} );
  V.addDualSpace(W);
  ASSERT_TRUE( V.isPrimalWRT(W) );
  ASSERT_FALSE( W.isPrimalWRT(V) );
}
