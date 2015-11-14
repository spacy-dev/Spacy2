#include <gtest/gtest.h>

#include "Test/mockSetup.hh"

using namespace Spacy;

auto& productSpaceCreator(VectorSpace& space)
{
  return Spacy::creator<ProductSpace::VectorCreator>(space);
}


TEST(PrimalDualProductSpaceCreator,IsPrimalProductSpace)
{
  auto t = makePrimalDualProductSpaceCreatorWithSpaceIndices();
  ASSERT_NO_THROW( Spacy::creator<ProductSpace::VectorCreator>(std::get<0>(t).subSpace(PRIMAL)) );
}

TEST(PrimalDualProductSpaceCreator,IsDualProductSpace)
{
  auto t = makePrimalDualProductSpaceCreatorWithSpaceIndices();
  ASSERT_NO_THROW( Spacy::creator<ProductSpace::VectorCreator>(std::get<0>(t).subSpace(DUAL)) );
}

TEST(PrimalDualSpaceCreator,IsPrimalProductSpace)
{
  auto t = makePrimalDualSpaceCreatorWithSpaceIndices();
  ASSERT_NO_THROW( Spacy::creator<Mock::VectorCreator>(std::get<0>(t).subSpace(DUAL)) );
}

TEST(PrimalDualSpaceCreator,IsDualProductSpace)
{
  auto t = makePrimalDualSpaceCreatorWithSpaceIndices();
  ASSERT_NO_THROW( Spacy::creator<Mock::VectorCreator>(std::get<0>(t).subSpace(PRIMAL)) );
}


TEST(PrimalDualProductSpaceCreator,PrimalSubSpace)
{
  auto t = makePrimalDualProductSpaceCreatorWithSpaceIndices();

  auto& primalCreator = productSpaceCreator( std::get<0>(t).subSpace(PRIMAL) );
  ASSERT_EQ( std::get<1>(t) , primalCreator.subSpace( 0 ).index() );
  ASSERT_EQ( std::get<2>(t) , primalCreator.subSpace( 1 ).index() );
}

TEST(PrimalDualSpaceCreator,PrimalSubSpace)
{
  auto t = makePrimalDualSpaceCreatorWithSpaceIndices();

  ASSERT_EQ( std::get<1>(t) , std::get<0>(t).subSpace(PRIMAL).index() );
}

TEST(PrimalDualProductSpaceCreator,DualSubSpace)
{
  auto t = makePrimalDualProductSpaceCreatorWithSpaceIndices();

  auto& dualCreator = productSpaceCreator( std::get<0>(t).subSpace(DUAL) );
  ASSERT_EQ( std::get<3>(t) , dualCreator.subSpace(0).index() );
}

TEST(PrimalDualSpaceCreator,DualSubSpace)
{
  auto t = makePrimalDualSpaceCreatorWithSpaceIndices();

  ASSERT_EQ( std::get<2>(t) , std::get<0>(t).subSpace(DUAL).index() );
}
