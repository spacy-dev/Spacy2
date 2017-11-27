#include "Operator.hh"

#include <Spacy/Spaces/ProductSpace/vector.hh>
#include <Spacy/Spaces/ProductSpace/vectorSpace.hh>
#include <Spacy/Util/cast.hh>
#include <Spacy/zeroVectorCreator.hh>

#include <numeric>

namespace Spacy
{
    namespace ProductSpace
    {
        Operator::Operator( const std::vector< std::vector< Spacy::Operator > >& blockOperators,
                            const Spacy::VectorSpace& domain, const Spacy::VectorSpace& range )
            : OperatorBase( domain, range ), blockOperators_( blockOperators )
        {
        }

        Spacy::Vector Operator::operator()( const ::Spacy::Vector& x ) const
        {
            auto y = zero( range() );
            const auto domain_is_product_space = is< Vector >( x );
            const auto range_is_product_space = is< Vector >( y );

            if ( domain_is_product_space && !range_is_product_space )
            {
                const auto& xp = cast_ref< Vector >( x );
                for ( auto j = 0u; j < xp.numberOfVariables(); ++j )
                    y += blockOperators_[ 0 ][ j ]( xp.component( j ) );
                return y;
            }

            if ( !domain_is_product_space && range_is_product_space )
            {
                auto& yp = cast_ref< Vector >( y );
                for ( auto i = 0u; i < yp.numberOfVariables(); ++i )
                    yp.component( i ) = blockOperators_[ i ][ 0 ]( x );
                return y;
            }

            if ( domain_is_product_space && range_is_product_space )
            {
                const auto& xp = cast_ref< Vector >( x );
                auto& yp = cast_ref< Vector >( y );
                for ( auto i = 0u; i < xp.numberOfVariables(); ++i )
                    for ( auto j = 0u; j < yp.numberOfVariables(); ++j )
                        yp.component( i ) += blockOperators_[ i ][ j ]( xp.component( j ) );
                return y;
            }

            return blockOperators_[ 0 ][ 0 ]( x );
        }

        Spacy::Operator& Operator::operator()( size_type row, size_type col )
        {
            return blockOperators_[ row ][ col ];
        }

        const Spacy::Operator& Operator::operator()( size_type row, size_type col ) const
        {
            return blockOperators_[ row ][ col ];
        }
    }
}
