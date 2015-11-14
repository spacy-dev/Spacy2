#include <gtest/gtest.h>

#include "Spacy/Algorithm/dampingFactor.hh"

TEST(DampingFactor,Assignment)
{
  auto nu = Spacy::DampingFactor{5.};

  EXPECT_DOUBLE_EQ( toDouble(nu()) , 5. );

  nu = 0.999;
  EXPECT_FALSE( nu() == 1 );

  nu = 0.9999;
  EXPECT_TRUE( nu() == 1 );
}
