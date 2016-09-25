#include <Test/gtest.hh>

#include <Test/mockSetup.hh>
#include <Spacy/Spaces/ProductSpace/norm.hh>

using namespace Spacy;

TEST(ProductSpaceNorm,Apply)
{
  auto norm = ProductSpace::Norm{};
  auto V = makeProductHilbertSpace();

  auto v = zero(std::get<0>(V));

  ASSERT_EQ( norm(v) , sqrt(2*Mock::Norm::testValue*Mock::Norm::testValue) );
}

