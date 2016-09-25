#include <Test/gtest.hh>

#include <Spacy/Algorithm/dampingFactor.hh>

TEST(DampingFactor,Assignment)
{
    auto eps = 1e-3;
    auto initial_value = 5.;
    auto nu = Spacy::DampingFactor{initial_value,eps};

    EXPECT_EQ( nu , initial_value );

    nu = 0.999;
    EXPECT_FALSE( nu == 1 );

    nu = 0.9999;
    EXPECT_TRUE( nu == 1 );
}
