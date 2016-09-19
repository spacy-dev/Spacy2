#include <gtest/gtest.h>

#include <Test/mockSetup.hh>
#include <Spacy/Spaces/ProductSpace/scalarProduct.hh>

using namespace Spacy;

TEST(ProductSpaceProduct,Apply)
{
  auto sp = ProductSpace::ScalarProduct{};
  auto V = makeProductHilbertSpace();

  auto v = zero(std::get<0>(V));

  EXPECT_EQ( sp(v,v) , 2*Mock::ScalarProduct::testValue );
}

TEST(ProductSpaceProduct,ApplyThrow)
{
  auto sp = ProductSpace::ScalarProduct{};
  auto V = makeProductHilbertSpace();
  auto W = makeProductHilbertSpace();

  auto v = zero(std::get<0>(V));
  auto w = zero(std::get<0>(W));

  EXPECT_THROW(sp(v,w), IncompatibleSpaceException);
}
