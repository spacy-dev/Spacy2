#include <Test/gtest.hh>

#include <Test/mockSetup.hh>
#include <Spacy/Spaces/ProductSpace/C1Operator.hh>
#include <Spacy/Spaces/ProductSpace/vector.hh>
#include <Spacy/Spaces/ProductSpace/vectorSpace.hh>
#include <Spacy/Spaces/ScalarSpace/c1Operator.hh>
#include <Spacy/Spaces/realSpace.hh>
#include <Spacy/Util/cast.hh>

#include <memory>
#include <vector>

using namespace Spacy;

namespace
{
    struct ProductSpaceC1Operator : testing::Test
    {
        std::vector< std::shared_ptr< VectorSpace > > two_spaces()
        {
            std::vector< std::shared_ptr< VectorSpace > > spaces;
            spaces.emplace_back( std::make_shared< VectorSpace >( make_real_space() ) );
            spaces.emplace_back( std::make_shared< VectorSpace >( make_real_space() ) );
            return spaces;
        }

        ProductSpaceC1Operator()
            : spaces( two_spaces() ), V( ProductSpace::makeHilbertSpace( spaces ) ),
              A11( []( double x ) { return x * x; }, []( double x ) { return 2 * x; }, *spaces[ 0 ],
                   *spaces[ 0 ] ),
              A12( []( double x ) { return x; }, []( double ) { return 1; }, *spaces[ 1 ],
                   *spaces[ 0 ] ),
              A21( []( double ) { return 1; }, []( double ) { return 0; }, *spaces[ 0 ],
                   *spaces[ 1 ] ),
              A22( []( double x ) { return x * x * x; }, []( double x ) { return 3 * x * x; },
                   *spaces[ 1 ], *spaces[ 1 ] )
        {
        }

        std::vector< std::shared_ptr< VectorSpace > > spaces;
        VectorSpace V;
        Scalar::C1Operator A11, A12, A21, A22;
    };
}

TEST_F( ProductSpaceC1Operator, Apply_2x2 )
{
    auto v = zero( V );
    auto& vp = cast_ref< ProductSpace::Vector >( v );
    cast_ref< Real >( vp.component( 0 ) ) = 1;
    cast_ref< Real >( vp.component( 1 ) ) = 2;

    auto A = ProductSpace::C1Operator( {{A11, A12}, {A21, A22}}, V, V );

    const auto w = A( v );
    const auto& wp = cast_ref< ProductSpace::Vector >( w );
    EXPECT_THAT( cast_ref< Real >( wp.component( 0 ) ), Real( 3, *spaces[ 0 ] ) );
    EXPECT_THAT( cast_ref< Real >( wp.component( 1 ) ), Real( 9, *spaces[ 1 ] ) );
}

TEST_F( ProductSpaceC1Operator, D1_2x2 )
{
    auto v = zero( V );
    auto& vp = cast_ref< ProductSpace::Vector >( v );
    cast_ref< Real >( vp.component( 0 ) ) = 1;
    cast_ref< Real >( vp.component( 1 ) ) = 2;

    auto dv = zero( V );
    auto& dvp = cast_ref< ProductSpace::Vector >( dv );
    cast_ref< Real >( dvp.component( 0 ) ) = 2;
    cast_ref< Real >( dvp.component( 1 ) ) = 2;

    auto A = ProductSpace::C1Operator( {{A11, A12}, {A21, A22}}, V, V );

    const auto x = A.d1( v, dv );
    const auto& xp = cast_ref< ProductSpace::Vector >( x );
    EXPECT_THAT( cast_ref< Real >( xp.component( 0 ) ), Real( 6, *spaces[ 0 ] ) );
    EXPECT_THAT( cast_ref< Real >( xp.component( 1 ) ), Real( 24, *spaces[ 1 ] ) );
}

TEST_F( ProductSpaceC1Operator, Apply_1x2 )
{
    auto v = zero( V );
    auto& vp = cast_ref< ProductSpace::Vector >( v );
    cast_ref< Real >( vp.component( 0 ) ) = 1;
    cast_ref< Real >( vp.component( 1 ) ) = 2;

    auto A = ProductSpace::C1Operator( {{A11, A12}}, V, *spaces[ 0 ] );

    const auto w = A( v );
    const auto& wr = cast_ref< Real >( w );
    EXPECT_THAT( get( wr ), 3 );
}

TEST_F( ProductSpaceC1Operator, D1_1x2 )
{
    auto v = zero( V );
    auto& vp = cast_ref< ProductSpace::Vector >( v );
    cast_ref< Real >( vp.component( 0 ) ) = 1;
    cast_ref< Real >( vp.component( 1 ) ) = 2;

    auto dv = zero( V );
    auto& dvp = cast_ref< ProductSpace::Vector >( dv );
    cast_ref< Real >( dvp.component( 0 ) ) = 2;
    cast_ref< Real >( dvp.component( 1 ) ) = 2;

    auto A = ProductSpace::C1Operator( {{A11, A12}}, V, *spaces[ 0 ] );

    const auto x = A.d1( v, dv );
    const auto& xr = cast_ref< Real >( x );
    EXPECT_THAT( get( xr ), 6 );
}

TEST_F( ProductSpaceC1Operator, Apply_2x1 )
{
    auto v = zero( *spaces[ 0 ] );
    cast_ref< Real >( v ) = 2;

    auto A = ProductSpace::C1Operator( {{A11}, {A21}}, *spaces[ 0 ], V );

    const auto w = A( v );
    const auto& wp = cast_ref< ProductSpace::Vector >( w );
    EXPECT_THAT( cast_ref< Real >( wp.component( 0 ) ), Real( 4, *spaces[ 0 ] ) );
    EXPECT_THAT( cast_ref< Real >( wp.component( 1 ) ), Real( 1, *spaces[ 1 ] ) );
}

TEST_F( ProductSpaceC1Operator, D1_2x1 )
{
    auto v = zero( *spaces[ 0 ] );
    cast_ref< Real >( v ) = 2;

    auto dv = zero( *spaces[ 0 ] );
    cast_ref< Real >( dv ) = 3;

    auto A = ProductSpace::C1Operator( {{A11}, {A21}}, *spaces[ 0 ], V );

    const auto x = A.d1( v, dv );
    const auto& xp = cast_ref< ProductSpace::Vector >( x );
    EXPECT_THAT( cast_ref< Real >( xp.component( 0 ) ), Real( 12, *spaces[ 0 ] ) );
    EXPECT_THAT( cast_ref< Real >( xp.component( 1 ) ), Real( 0, *spaces[ 1 ] ) );
}
