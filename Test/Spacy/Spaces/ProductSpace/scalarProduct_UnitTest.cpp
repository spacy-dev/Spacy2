#include <gtest/gtest.h>

#include "Test/mockSetup.hh"
#include "Spacy/Spaces/ProductSpace/scalarProduct.hh"

using namespace Spacy;

TEST(ProductSpaceProduct,Apply)
{
  auto sp = ProductSpace::ScalarProduct{};
  auto V = makeProductHilbertSpace();

  auto v = std::get<0>(V).zeroVector();

  ASSERT_EQ( sp(v,v) , 2*Mock::ScalarProduct::testValue );
}

TEST(ProductSpaceProduct,ApplyThrow)
{
  auto sp = ProductSpace::ScalarProduct{};
  auto V = makeProductHilbertSpace();
  auto W = makeProductHilbertSpace();

  auto v = std::get<0>(V).zeroVector();
  auto w = std::get<0>(W).zeroVector();

  ASSERT_THROW(sp(v,w), IncompatibleSpaceException);
}
