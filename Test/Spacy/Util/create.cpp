#include <gtest/gtest.h>

#include <Spacy/Util/invoke.hh>
#include <Spacy/Algorithm/Scalar/quadratic.hh>

namespace
{
    auto makeQuadratic(double a, double b, double c)
    {
        return Spacy::Quadratic(a,b,c);
    }
}

TEST(Invoke,MakeQuadratic)
{
    auto args = std::make_tuple(3,2,1);
    auto f = Spacy::invoke(makeQuadratic,args);

    EXPECT_EQ( f(2) , 11 );
}

TEST(Create,CreateQuadratic)
{
    auto args = std::make_tuple(3,2,1);
    auto f = Spacy::create<Spacy::Quadratic>(args);

    EXPECT_EQ( f(2) , 11 );
}
