#include <Test/gtest.hh>

#include <Spacy/Adapter/Eigen/vectorCreator.hh>
#include <Test/mockSetup.hh>
#include "setup.hh"

using namespace Spacy;

TEST(Rn,SingleSpaceCreator)
{
  auto V = Rn::makeHilbertSpace( testDim() );
  EXPECT_EQ( creator<Rn::VectorCreator>(V).dim() , testDim() );
}
