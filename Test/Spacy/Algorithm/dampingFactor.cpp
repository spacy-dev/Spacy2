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


const auto two = Spacy::Real{2};
const auto three = Spacy::Real{3};
const auto five = Spacy::Real{5};
const auto six = Spacy::Real{6};
const auto two_third = Spacy::Real{2.0/3};
const auto minus_one = Spacy::Real{-1};

const auto two_damping = Spacy::DampingFactor{two};
const auto three_damping = Spacy::DampingFactor{three};
const auto five_damping = Spacy::DampingFactor{five};
const auto six_damping = Spacy::DampingFactor{six};
const auto two_third_damping = Spacy::DampingFactor{two_third};
const auto minus_one_damping = Spacy::DampingFactor{minus_one};


// Comparison
TEST(DampingFactor, LessThanComparison)
{
    const auto two_damping_is_smaller_than_three = two_damping < three;
    EXPECT_TRUE( two_damping_is_smaller_than_three );
    const auto two_is_smaller_than_three_damping = two < three_damping;
    EXPECT_TRUE( two_is_smaller_than_three_damping );
    const auto two_damping_is_smaller_than_three_damping = two_damping < three_damping;
    EXPECT_TRUE( two_damping_is_smaller_than_three_damping );

    const auto five_damping_is_smaller_than_three = five_damping < three;
    EXPECT_FALSE( five_damping_is_smaller_than_three );
    const auto five_is_smaller_than_three_damping = five < three_damping;
    EXPECT_FALSE( five_is_smaller_than_three_damping );
    const auto five_damping_is_smaller_than_three_damping = five_damping < three_damping;
    EXPECT_FALSE( five_damping_is_smaller_than_three_damping );
}

TEST(DampingFactor, LessThanOrEqualsComparison)
{
    const auto two_damping_is_smaller_or_equal_than_three = two_damping <= three;
    EXPECT_TRUE( two_damping_is_smaller_or_equal_than_three );
    const auto two_is_smaller_or_equal_than_three_damping = two <= three_damping;
    EXPECT_TRUE( two_is_smaller_or_equal_than_three_damping );
    const auto two_damping_is_smaller_or_equal_than_three_damping = two_damping <= three_damping;
    EXPECT_TRUE( two_damping_is_smaller_or_equal_than_three_damping );

    const auto three_damping_is_smaller_or_equal_than_three = three_damping <= three;
    EXPECT_TRUE( three_damping_is_smaller_or_equal_than_three );
    const auto three_is_smaller_or_equal_than_three_damping = three <= three_damping;
    EXPECT_TRUE( three_is_smaller_or_equal_than_three_damping );
    const auto three_damping_is_smaller_or_equal_than_three_damping = three_damping <= three_damping;
    EXPECT_TRUE( three_damping_is_smaller_or_equal_than_three_damping );

    const auto five_damping_is_smaller_or_equal_than_three = five_damping <= three;
    EXPECT_FALSE( five_damping_is_smaller_or_equal_than_three );
    const auto five_is_smaller_or_equal_than_three_damping = five <= three_damping;
    EXPECT_FALSE( five_is_smaller_or_equal_than_three_damping );
    const auto five_damping_is_smaller_or_equal_than_three_damping = five_damping <= three_damping;
    EXPECT_FALSE( five_damping_is_smaller_or_equal_than_three_damping );
}

TEST(DampingFactor, BiggerThanComparison)
{
    const auto two_damping_is_bigger_than_three = two_damping > three;
    EXPECT_FALSE( two_damping_is_bigger_than_three );
    const auto two_is_bigger_than_three_damping = two > three_damping;
    EXPECT_FALSE( two_is_bigger_than_three_damping );
    const auto two_damping_is_bigger_than_three_damping = two_damping > three_damping;
    EXPECT_FALSE( two_damping_is_bigger_than_three_damping );

    const auto five_damping_is_bigger_than_three = five_damping > three;
    EXPECT_TRUE( five_damping_is_bigger_than_three );
    const auto five_is_bigger_than_three_damping = five > three_damping;
    EXPECT_TRUE( five_is_bigger_than_three_damping );
    const auto five_damping_is_bigger_than_three_damping = five_damping > three_damping;
    EXPECT_TRUE( five_damping_is_bigger_than_three_damping );
}

TEST(DampingFactor, BiggerThanOrEqualsComparison)
{
    const auto two_damping_is_bigger_or_equal_than_three = two_damping >= three;
    EXPECT_FALSE( two_damping_is_bigger_or_equal_than_three );
    const auto two_is_bigger_or_equal_than_three_damping = two >= three_damping;
    EXPECT_FALSE( two_is_bigger_or_equal_than_three_damping );
    const auto two_damping_is_bigger_or_equal_than_three_damping = two_damping >= three_damping;
    EXPECT_FALSE( two_damping_is_bigger_or_equal_than_three_damping );

    const auto three_damping_is_bigger_or_equal_than_three = three_damping >= three;
    EXPECT_TRUE( three_damping_is_bigger_or_equal_than_three );
    const auto three_is_bigger_or_equal_than_three_damping = three >= three_damping;
    EXPECT_TRUE( three_is_bigger_or_equal_than_three_damping );
    const auto three_damping_is_bigger_or_equal_than_three_damping = three_damping >= three_damping;
    EXPECT_TRUE( three_damping_is_bigger_or_equal_than_three_damping );

    const auto five_damping_is_bigger_or_equal_than_three = five_damping >= three;
    EXPECT_TRUE( five_damping_is_bigger_or_equal_than_three );
    const auto five_is_bigger_or_equal_than_three_damping = five >= three_damping;
    EXPECT_TRUE( five_is_bigger_or_equal_than_three_damping );
    const auto five_damping_is_bigger_or_equal_than_three_damping = five_damping >= three_damping;
    EXPECT_TRUE( five_damping_is_bigger_or_equal_than_three_damping );
}

TEST(DampingFactor, Equals)
{
    const auto two_damping_equals_three = two_damping == three;
    EXPECT_FALSE( two_damping_equals_three );
    const auto two_equals_three_damping = two == three_damping;
    EXPECT_FALSE( two_equals_three_damping );
    const auto two_damping_equals_three_damping = two_damping == three_damping;
    EXPECT_FALSE( two_damping_equals_three_damping );

    const auto two_damping_equals_two = two_damping == two;
    EXPECT_TRUE( two_damping_equals_two );
    const auto two_equals_two_damping = two == two_damping;
    EXPECT_TRUE( two_equals_two_damping );
    const auto two_damping_equals_two_damping = two_damping == two_damping;
    EXPECT_TRUE( two_damping_equals_two_damping );
}

// Arithmetic operations
TEST(DampingFactor, TwoPlusThreeEqualsFive)
{
    const auto two_damping_plus_three = two_damping + three;
    const auto typeOf_two_damping_plus_three_IsDampingFactor =
            std::is_same<decltype(two_damping_plus_three),const Spacy::DampingFactor>::value;
    EXPECT_TRUE(typeOf_two_damping_plus_three_IsDampingFactor);
    EXPECT_EQ( two_damping_plus_three, five_damping );

    const auto two_plus_three_damping = two + three_damping;
    const auto typeOf_two_plus_three_damping_IsReal =
            std::is_same<decltype(two_plus_three_damping),const Spacy::Real>::value;
    EXPECT_TRUE(typeOf_two_plus_three_damping_IsReal);
    EXPECT_EQ( two_plus_three_damping, five );

    const auto two_damping_plus_three_damping = two_damping + three_damping;
    const auto typeOf_two_damping_plus_three_damping_IsDampingFactor =
            std::is_same<decltype(two_damping_plus_three_damping),const Spacy::DampingFactor>::value;
    EXPECT_TRUE(typeOf_two_damping_plus_three_damping_IsDampingFactor);
    EXPECT_EQ( two_damping_plus_three_damping, five_damping );
}

TEST(DampingFactor, TwoMinusThreeEqualsMinusOne)
{
    const auto two_damping_minus_three = two_damping - three;
    const auto typeOf_two_damping_minus_three_IsDampingFactor =
            std::is_same<decltype(two_damping_minus_three),const Spacy::DampingFactor>::value;
    EXPECT_TRUE(typeOf_two_damping_minus_three_IsDampingFactor);
    EXPECT_EQ( two_damping_minus_three, minus_one_damping );

    const auto two_minus_three_damping = two - three_damping;
    const auto typeOf_two_minus_three_damping_IsReal =
            std::is_same<decltype(two_minus_three_damping),const Spacy::Real>::value;
    EXPECT_TRUE(typeOf_two_minus_three_damping_IsReal);
    EXPECT_EQ( two_minus_three_damping, minus_one );

    const auto two_damping_minus_three_damping = two_damping - three_damping;
    const auto typeOf_two_damping_minus_three_damping_IsDampingFactor =
            std::is_same<decltype(two_damping_minus_three_damping),const Spacy::DampingFactor>::value;
    EXPECT_TRUE(typeOf_two_damping_minus_three_damping_IsDampingFactor);
    EXPECT_EQ( two_damping_minus_three_damping, minus_one_damping );
}

TEST(DampingFactor, TwoTimesThreeEqualsSix)
{
    const auto two_damping_times_three = two_damping * three;
    const auto typeOf_two_damping_times_three_IsReal =
            std::is_same<decltype(two_damping_times_three),const Spacy::Real>::value;
    EXPECT_TRUE(typeOf_two_damping_times_three_IsReal);
    EXPECT_EQ( two_damping_times_three, six_damping );

    const auto two_times_three_damping = two * three_damping;
    const auto typeOf_two_times_three_damping_IsReal =
            std::is_same<decltype(two_times_three_damping),const Spacy::Real>::value;
    EXPECT_TRUE(typeOf_two_times_three_damping_IsReal);
    EXPECT_EQ( two_times_three_damping, six_damping );

    const auto two_damping_times_three_damping = two_damping * three_damping;
    const auto typeOf_two_damping_times_three_damping_IsDampingFactor =
            std::is_same<decltype(two_damping_times_three_damping),const Spacy::DampingFactor>::value;
    EXPECT_TRUE(typeOf_two_damping_times_three_damping_IsDampingFactor);
    EXPECT_EQ( two_damping_times_three_damping, six_damping );
}

TEST(DampingFactor, TwoDivThreeEqualsTwoThird)
{
    const auto two_damping_div_three = two_damping / three;
    const auto typeOf_two_damping_div_three_IsReal =
            std::is_same<decltype(two_damping_div_three),const Spacy::Real>::value;
    EXPECT_TRUE(typeOf_two_damping_div_three_IsReal);
    EXPECT_EQ( two_damping_div_three, two_third_damping );

    const auto two_div_three_damping = two / three_damping;
    const auto typeOf_two_div_three_damping_IsReal =
            std::is_same<decltype(two_div_three_damping),const Spacy::Real>::value;
    EXPECT_TRUE(typeOf_two_div_three_damping_IsReal);
    EXPECT_EQ( two_div_three_damping, two_third );

    const auto two_damping_div_three_damping = two_damping / three_damping;
    const auto typeOf_two_damping_div_three_damping_IsDampingFactor =
            std::is_same<decltype(two_damping_div_three_damping),const Spacy::DampingFactor>::value;
    EXPECT_TRUE(typeOf_two_damping_div_three_damping_IsDampingFactor);
    EXPECT_EQ( two_damping_div_three_damping, two_third_damping );
}
