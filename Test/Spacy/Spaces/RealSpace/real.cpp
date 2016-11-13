#include <Test/gtest.hh>

#include <Spacy/Spaces/RealSpace/real.hh>

const auto two = Spacy::Real{2};
const auto three = Spacy::Real{3};
const auto five = Spacy::Real{5};
const auto six = Spacy::Real{6};
const auto two_third = Spacy::Real{2.0/3};
const auto minus_one = Spacy::Real{-1};

// Comparison
TEST(Real, LessThanComparison)
{
    const auto real_two_is_smaller_than_real_three = two < three;
    EXPECT_TRUE( real_two_is_smaller_than_real_three );
    const auto real_two_is_smaller_than_three = two < 3;
    EXPECT_TRUE( real_two_is_smaller_than_three );
    const auto two_is_smaller_than_real_three = 2 < three;
    EXPECT_TRUE( two_is_smaller_than_real_three );

    const auto real_five_is_smaller_than_real_three = five < three;
    EXPECT_FALSE( real_five_is_smaller_than_real_three );
    const auto real_five_is_smaller_than_three = five < 3;
    EXPECT_FALSE( real_five_is_smaller_than_three );
    const auto five_is_smaller_than_real_three = 5 < three;
    EXPECT_FALSE( five_is_smaller_than_real_three );
}

TEST(Real, LessThanOrEqualsComparison)
{
    const auto real_two_is_smaller_or_equal_than_real_three = two <= three;
    EXPECT_TRUE( real_two_is_smaller_or_equal_than_real_three );
    const auto real_two_is_smaller_or_equal_than_three = two <= 3;
    EXPECT_TRUE( real_two_is_smaller_or_equal_than_three );
    const auto two_is_smaller_or_equal_than_real_three = 2 <= three;
    EXPECT_TRUE( two_is_smaller_or_equal_than_real_three );

    const auto real_three_is_smaller_or_equal_than_real_three = three <= three;
    EXPECT_TRUE( real_three_is_smaller_or_equal_than_real_three );
    const auto real_three_is_smaller_or_equal_than_three = three <= 3;
    EXPECT_TRUE( real_three_is_smaller_or_equal_than_three );
    const auto three_is_smaller_or_equal_than_real_three = 3 <= three;
    EXPECT_TRUE( three_is_smaller_or_equal_than_real_three );

    const auto real_five_is_smaller_or_equal_than_real_three = five <= three;
    EXPECT_FALSE( real_five_is_smaller_or_equal_than_real_three );
    const auto real_five_is_smaller_or_equal_than_three = five <= 3;
    EXPECT_FALSE( real_five_is_smaller_or_equal_than_three );
    const auto five_is_smaller_or_equal_than_real_three = 5 <= three;
    EXPECT_FALSE( five_is_smaller_or_equal_than_real_three );
}

TEST(Real, BiggerThanComparison)
{
    const auto real_two_is_bigger_than_real_three = two > three;
    EXPECT_FALSE( real_two_is_bigger_than_real_three );
    const auto real_two_is_bigger_than_three = two > 3;
    EXPECT_FALSE( real_two_is_bigger_than_three );
    const auto two_is_bigger_than_real_three = 2 > three;
    EXPECT_FALSE( two_is_bigger_than_real_three );

    const auto real_five_is_bigger_than_real_three = five > three;
    EXPECT_TRUE( real_five_is_bigger_than_real_three );
    const auto real_five_is_bigger_than_three = five > 3;
    EXPECT_TRUE( real_five_is_bigger_than_three );
    const auto five_is_bigger_than_real_three = 5 > three;
    EXPECT_TRUE( five_is_bigger_than_real_three );
}

TEST(Real, BiggerThanOrEqualsComparison)
{
    const auto real_two_is_bigger_or_equal_than_real_three = two >= three;
    EXPECT_FALSE( real_two_is_bigger_or_equal_than_real_three );
    const auto real_two_is_bigger_or_equal_than_three = two >= 3;
    EXPECT_FALSE( real_two_is_bigger_or_equal_than_three );
    const auto two_is_bigger_or_equal_than_real_three = 2 >= three;
    EXPECT_FALSE( two_is_bigger_or_equal_than_real_three );

    const auto real_three_is_bigger_or_equal_than_real_three = three >= three;
    EXPECT_TRUE( real_three_is_bigger_or_equal_than_real_three );
    const auto real_three_is_bigger_or_equal_than_three = three >= 3;
    EXPECT_TRUE( real_three_is_bigger_or_equal_than_three );
    const auto three_is_bigger_or_equal_than_real_three = 3 >= three;
    EXPECT_TRUE( three_is_bigger_or_equal_than_real_three );

    const auto real_five_is_bigger_or_equal_than_real_three = five >= three;
    EXPECT_TRUE( real_five_is_bigger_or_equal_than_real_three );
    const auto real_five_is_bigger_or_equal_than_three = five >= 3;
    EXPECT_TRUE( real_five_is_bigger_or_equal_than_three );
    const auto five_is_bigger_or_equal_than_real_three = 5 >= three;
    EXPECT_TRUE( five_is_bigger_or_equal_than_real_three );
}

TEST(Real, Equals)
{
    const auto real_two_equals_real_three = two == three;
    EXPECT_FALSE( real_two_equals_real_three );
    const auto real_two_equals_three = two == 3;
    EXPECT_FALSE( real_two_equals_three );
    const auto two_equals_real_three = 2 == three;
    EXPECT_FALSE( two_equals_real_three );

    const auto real_two_equals_real_two = two == two;
    EXPECT_TRUE( real_two_equals_real_two );
    const auto real_two_equals_two = two == 2;
    EXPECT_TRUE( real_two_equals_two );
    const auto two_equals_real_two = 2 == two;
    EXPECT_TRUE( two_equals_real_two );
}

// Arithmetic operations
TEST(Real, TwoPlusThreeEqualsFive)
{
    const auto real_two_plus_real_three = two + three;
    EXPECT_EQ( real_two_plus_real_three, five );
    const auto real_two_plus_three = two + 3;
    EXPECT_EQ( real_two_plus_three, five );
    const auto two_plus_real_three = 2 + three;
    EXPECT_EQ( two_plus_real_three, five );
}

TEST(Real, TwoMinusThreeEqualsMinusOne)
{
    const auto real_two_minus_real_three = two - three;
    EXPECT_EQ( real_two_minus_real_three, minus_one );
    const auto real_two_minus_three = two - 3;
    EXPECT_EQ( real_two_minus_three, minus_one );
    const auto two_minus_real_three = 2 - three;
    EXPECT_EQ( two_minus_real_three, minus_one );
}

TEST(Real, TwoTimesThreeEqualsSix)
{
    const auto real_two_times_real_three = two * three;
    EXPECT_EQ( real_two_times_real_three, six );
    const auto real_two_times_three = two * 3;
    EXPECT_EQ( real_two_times_three, six );
    const auto two_times_real_three = 2 * three;
    EXPECT_EQ( two_times_real_three, six );
}

TEST(Real, TwoDivThreeEqualsTwoThird)
{
    const auto two_div_three = two / three;
    EXPECT_EQ( two_div_three, two_third );
}
