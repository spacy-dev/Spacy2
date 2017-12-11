#include "Norm.h"

#include <Spacy/Norm.h>
#include <Spacy/Spaces/ScalarSpace/Real.hh>
#include <Spacy/Util/cast.hh>
#include <Spacy/vector.hh>

#include "vector.hh"
#include "vectorSpace.hh"

#include <cmath>

namespace Spacy
{
    namespace ProductSpace
    {
        Real Norm::operator()( const ::Spacy::Vector& x ) const
        {
            const auto& x_ = cast_ref< Vector >( x );

            auto result = Real{0.};
            for ( auto i = 0u; i < x_.numberOfVariables(); ++i )
            {
                auto nx = norm( x_.component( i ) );
                result += nx * nx;
            }
            return sqrt( result );
        }
    }
}
