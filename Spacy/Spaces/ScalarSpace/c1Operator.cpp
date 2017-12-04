#include "c1Operator.hh"

#include <Spacy/Spaces/realSpace.hh>
#include <Spacy/Util/Exceptions/callOfUndefinedFunctionException.hh>
#include <Spacy/Util/cast.hh>
#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>
#include <Spacy/zeroVectorCreator.hh>

#include "LinearOperator.hh"

#include <cassert>

namespace Spacy
{
    namespace Scalar
    {
        C1Operator::C1Operator( std::function< double( double ) > value,
                                std::function< double( double ) > derivative )
            : C1Operator( std::move( value ), std::move( derivative ), Space::R, Space::R )
        {
        }

        C1Operator::C1Operator( std::function< double( double ) > value,
                                std::function< double( double ) > derivative,
                                const VectorSpace& domain, const VectorSpace& range )
            : OperatorBase( domain, range ), value_( value ), derivative_( derivative ),
              operatorSpace_( std::make_shared< VectorSpace >(
                  []( const ::Spacy::VectorSpace* ) -> Spacy::Vector {
                      throw CallOfUndefinedFunctionException(
                          "OperatorCreator::operator()(const VectorSpace*)" );
                  },
                  []( const ::Spacy::Vector& ) -> Spacy::Real {
                      throw CallOfUndefinedFunctionException( "LinearOperatorNorm" );
                  },
                  true ) )
        {
        }

        Spacy::Vector C1Operator::operator()( const ::Spacy::Vector& x ) const
        {
            assert( value_ );
            return Real( value_( get( cast_ref< Real >( x ) ) ), range() );
        }

        Spacy::Vector C1Operator::d1( const ::Spacy::Vector& x, const ::Spacy::Vector& dx ) const
        {
            assert( derivative_ );
            return Real( derivative_( get( cast_ref< Real >( x ) ) ), range() ) *
                   get( cast_ref< Real >( dx ) );
        }

        LinearOperator C1Operator::linearization( const ::Spacy::Vector& x ) const
        {
            assert( derivative_ );
            assert( operatorSpace_ != nullptr );
            return LinearOperator( *operatorSpace_, derivative_( get( cast_ref< Real >( x ) ) ) );
        }
    }
}
