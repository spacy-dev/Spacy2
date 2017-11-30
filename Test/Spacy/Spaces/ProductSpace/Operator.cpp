#include <Test/gtest.hh>

#include <Test/mockSetup.hh>
#include <Spacy/Spaces/ProductSpace/Operator.hh>
#include <Spacy/Spaces/ProductSpace/vector.hh>
#include <Spacy/Spaces/ScalarSpace/Operator.hh>
#include <Spacy/Spaces/realSpace.hh>
#include <Spacy/Util/cast.hh>

#include <memory>
#include <vector>

using namespace Spacy;

TEST( ProductSpaceOperator, Apply_2x2 )
{
    std::vector< std::shared_ptr< VectorSpace > > spaces;
    spaces.emplace_back( std::make_shared< VectorSpace >( make_real_space() ) );
    spaces.emplace_back( std::make_shared< VectorSpace >( make_real_space() ) );
    auto V = ProductSpace::makeHilbertSpace( spaces );
    auto v = zero( V );
    auto& vp = cast_ref< ProductSpace::Vector >( v );
    cast_ref< Real >( vp.component( 0 ) ) = 1;
    cast_ref< Real >( vp.component( 1 ) ) = 2;

    Scalar::Operator A11( []( double a ) { return a; }, *spaces[ 0 ], *spaces[ 0 ] );
    Scalar::Operator A12( []( double a ) { return -a; }, *spaces[ 1 ], *spaces[ 0 ] );
    Scalar::Operator A21( []( double a ) { return -a; }, *spaces[ 0 ], *spaces[ 1 ] );
    Scalar::Operator A22( []( double a ) { return 2 * a; }, *spaces[ 1 ], *spaces[ 1 ] );

    auto A = ProductSpace::Operator( {{A11, A12}, {A21, A22}}, V, V );

    auto w = A( v );
    const auto& wp = cast_ref< ProductSpace::Vector >( w );
    EXPECT_THAT( cast_ref< Real >( wp.component( 0 ) ), Real( -1, *spaces[ 0 ] ) );
    EXPECT_THAT( cast_ref< Real >( wp.component( 1 ) ), Real( 3, *spaces[ 1 ] ) );
}

TEST( ProductSpaceOperator, Apply_1x2 )
{
    std::vector< std::shared_ptr< VectorSpace > > spaces;
    spaces.emplace_back( std::make_shared< VectorSpace >( make_real_space() ) );
    spaces.emplace_back( std::make_shared< VectorSpace >( make_real_space() ) );
    auto V = ProductSpace::makeHilbertSpace( spaces );
    auto v = zero( V );
    auto& vp = cast_ref< ProductSpace::Vector >( v );
    cast_ref< Real >( vp.component( 0 ) ) = 1;
    cast_ref< Real >( vp.component( 1 ) ) = 2;

    Scalar::Operator A11( []( double a ) { return a; }, *spaces[ 0 ], *spaces[ 0 ] );
    Scalar::Operator A12( []( double a ) { return -a; }, *spaces[ 1 ], *spaces[ 0 ] );

    auto A = ProductSpace::Operator( {{A11, A12}}, V, *spaces[ 0 ] );

    auto w = A( v );
    const auto& wr = cast_ref< Real >( w );
    EXPECT_THAT( get( wr ), -1.0 );
}

TEST( ProductSpaceOperator, Apply_2x1 )
{
    std::vector< std::shared_ptr< VectorSpace > > spaces;
    spaces.emplace_back( std::make_shared< VectorSpace >( make_real_space() ) );
    spaces.emplace_back( std::make_shared< VectorSpace >( make_real_space() ) );
    auto V = ProductSpace::makeHilbertSpace( spaces );
    auto v = zero( *spaces[ 0 ] );
    cast_ref< Real >( v ) = 2;

    Scalar::Operator A11( []( double a ) { return a; }, *spaces[ 0 ], *spaces[ 0 ] );
    Scalar::Operator A21( []( double a ) { return -a; }, *spaces[ 0 ], *spaces[ 1 ] );

    auto A = ProductSpace::Operator( {{A11}, {A21}}, *spaces[ 0 ], V );

    auto w = A( v );
    const auto& wp = cast_ref< ProductSpace::Vector >( w );
    EXPECT_THAT( cast_ref< Real >( wp.component( 0 ) ), Real( 2, *spaces[ 0 ] ) );
    EXPECT_THAT( cast_ref< Real >( wp.component( 1 ) ), Real( -2, *spaces[ 1 ] ) );
}
