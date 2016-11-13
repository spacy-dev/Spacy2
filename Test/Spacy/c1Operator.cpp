#include <Test/gtest.hh>

#include <Spacy/operator.hh>
#include <Spacy/c1Operator.hh>
#include <Spacy/Spaces/realSpace.hh>
#include <Spacy/Util/cast.hh>
#include <Test/mockSetup.hh>
#include <Test/Mock/linearOperator.hh>

using namespace Spacy;

namespace
{
    struct TestC1Operator
    {
        TestC1Operator(const VectorSpace& domain, const VectorSpace& range)
            : domain_(&domain), range_(&range)
        {}

        Vector operator()(const Vector&) const
        {
            return Real(3.);
        }

        Vector d1(const Vector&,const Vector&) const
        {
            return Real(2.);
        }

        LinearOperator linearization(const Vector&) const
        {
            return Mock::LinearOperator();
        }

        const VectorSpace& domain() const
        {
            return *domain_;
        }

        const VectorSpace& range() const
        {
            return *range_;
        }

    private:
        const VectorSpace* domain_;
        const VectorSpace* range_;
    };

    void test(const C1Operator& f, const VectorSpace& X, const VectorSpace& Y)
    {
        EXPECT_DOUBLE_EQ( get(cast_ref<Real>(f(zero(X)))) , 3 );
        EXPECT_DOUBLE_EQ( get(cast_ref<Real>(f.d1(zero(X),zero(X)))) , 2 );
        EXPECT_EQ( X.index() , f.domain().index() );
        EXPECT_EQ( Y.index() , f.range().index() );

        auto L = f.linearization(zero(X));
        auto expected = is<Mock::LinearOperator>(L);
        EXPECT_TRUE(expected);
    }

    void testOp(const Operator& f, const VectorSpace& X, const VectorSpace& Y)
    {
        EXPECT_DOUBLE_EQ( get(cast_ref<Real>(f(zero(X)))) , 3 );
        EXPECT_EQ( X.index() , f.domain().index() );
        EXPECT_EQ( Y.index() , f.range().index() );
    }
}


TEST(C1Operator,Assert)
{
    auto X = createMockBanachSpace();
    C1Operator f;
    ASSERT_DEATH( f(zero(X)) , "" );
    ASSERT_DEATH( f.d1(zero(X),zero(X)) , "" );
    ASSERT_DEATH( f.linearization(zero(X)) , "" );
    ASSERT_DEATH( f.domain() , "" );
    ASSERT_DEATH( f.range() , "" );
}

TEST(C1Operator,IsEmpty)
{
    auto X = createMockBanachSpace();
    auto Y = createMockBanachSpace();
    C1Operator f;
    C1Operator g = TestC1Operator(X,Y);

    bool f_is_empty = !f;
    bool g_is_empty = !g;
    ASSERT_TRUE( f_is_empty );
    ASSERT_FALSE( g_is_empty );
}

TEST(C1Operator,Cast)
{
    auto X = createMockBanachSpace();
    auto Y = createMockBanachSpace();
    C1Operator f = TestC1Operator(X,Y);

    bool validCast = f.template target<TestC1Operator>() != nullptr;
    ASSERT_TRUE( validCast );
}

TEST(C1Operator,StoreRValue)
{
    auto X = createMockBanachSpace();
    auto Y = createMockBanachSpace();
    C1Operator f = TestC1Operator(X,Y);

    test(f,X,Y);
}

TEST(C1Operator,StoreCopy)
{
    auto X = createMockBanachSpace();
    auto Y = createMockBanachSpace();
    auto g = TestC1Operator(X,Y);
    C1Operator f = g;

    test(f,X,Y);
    test(g,X,Y);
}

TEST(C1Operator,Copy)
{
    auto X = createMockBanachSpace();
    auto Y = createMockBanachSpace();
    C1Operator g = TestC1Operator(X,Y);
    C1Operator f = g;

    test(f,X,Y);
    test(g,X,Y);
}

TEST(C1Operator,Move)
{
    auto X = createMockBanachSpace();
    auto Y = createMockBanachSpace();
    C1Operator g = TestC1Operator(X,Y);
    bool is_empty_before_move = !g;
    C1Operator f = std::move(g);
    bool is_empty_after_move = !g;

    EXPECT_FALSE(is_empty_before_move);
    EXPECT_TRUE(is_empty_after_move);

    test(f,X,Y);
}

TEST(C1Operator,ToOperator)
{
    auto X = createMockBanachSpace();
    auto Y = createMockBanachSpace();
    C1Operator g = TestC1Operator(X,Y);
    C1Operator h = TestC1Operator(X,Y);

    Operator copied = g;
    const Operator& bound = h;
    auto fun  = [&X](const Operator& A) { return A(zero(X)); };
    auto x = fun(h);
    Operator moved = std::move(g);
    testOp(copied,X,Y);
    testOp(moved,X,Y);
    testOp(bound,X,Y);
}


