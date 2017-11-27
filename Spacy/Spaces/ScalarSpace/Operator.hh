#pragma once

#include <functional>

#include <Spacy/Util/Base/OperatorBase.hh>

namespace Spacy
{
    /// @cond
    class Vector;
    /// @endcond

    /** @addtogroup ScalarGroup @{ */
    namespace Scalar
    {
        /// Operator \f$A:\mathbb{R}\rightarrow\mathbb{R}\f$.
        class Operator : public Spacy::OperatorBase
        {
        public:
            /// Constructor for operators between the global real space (index = 0).
            explicit Operator( std::function< double( double ) > value );

            /// Constructor for general operators between real spaces.
            Operator( std::function< double( double ) > value, const VectorSpace& domain,
                      const VectorSpace& range );

            /// Compute \f$A(x)\in\mathbb{R}\f$.
            Spacy::Vector operator()( const ::Spacy::Vector& x ) const;

        private:
            std::function< double( double ) > value_;
        };
    }
    /** @} */
}
