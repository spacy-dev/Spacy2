#include <Test/gtest.hh>

#include <Spacy/Algorithm/Scalar/cubic.hh>

TEST(Algorithm_Scalar_Cubic,CreateAndApply)
{
  auto f = Spacy::Scalar::Cubic(4,3,2,1);

  EXPECT_EQ( f(2) , 26 );
}


