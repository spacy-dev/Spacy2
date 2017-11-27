#pragma once

#include <Spacy/Util/Base/OperatorBase.hh>
#include <Spacy/operator.hh>

#include <vector>

namespace Spacy
{
    namespace ProductSpace
    {
        /**
         * @brief Product space operator.
         *
         * The structure of this operator must be consistently defined through the (block-)operators
         * and
         * the domain and range space.
         */
        class Operator : public OperatorBase
        {
        public:
            using size_type = std::vector< std::vector< Spacy::Operator > >::size_type;

            /**
             * @brief Constructor.
             *
             * blockOperators and domain/range must be consistent, i.e. if range is a product space,
             * then the number of components in this space (ignoring subspaces) must coincide with
             * the
             * size of blockOperators. The same holds for domain wrt. each of blockOperators[i],
             * i...
             */
            Operator( const std::vector< std::vector< Spacy::Operator > >& blockOperators,
                      const Spacy::VectorSpace& domain, const Spacy::VectorSpace& range );

            /// Compute \f$A(x)\in\mathbb{R}\f$.
            Spacy::Vector operator()( const Spacy::Vector& x ) const;

            /// Access \f$A_{row,col}\f$.
            Spacy::Operator& operator()( size_type row, size_type col );

            /// Access \f$A_{row,col}\f$ (read-only).
            const Spacy::Operator& operator()( size_type row, size_type col ) const;

        private:
            std::vector< std::vector< Spacy::Operator > > blockOperators_;
        };
    }
}
