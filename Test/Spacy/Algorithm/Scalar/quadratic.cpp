#include <gtest/gtest.h>

#include <Spacy/Algorithm/Scalar/quadratic.hh>

TEST(Algorithm_Scalar_Quadratic,CreateAndApply)
{
  auto f = Spacy::Quadratic(3,2,1);

  EXPECT_EQ( f(2) , 11 );
}

