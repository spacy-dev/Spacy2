#pragma once

#include <Spacy/Util/Base/OperatorBase.hh>
#include <Spacy/c1Operator.hh>
#include <Spacy/operator.hh>

#include <vector>
#include <functional>

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
        class C1Operator : public OperatorBase
        {
        public:
            using size_type = std::vector< std::vector< Spacy::C1Operator > >::size_type;

            /**
             * @brief Constructor.
             *
             * blockOperators and domain/range must be consistent, i.e. if range is a product space,
             * then the number of components in this space (ignoring subspaces) must coincide with
             * the
             * size of blockOperators. The same holds for domain wrt. each of blockOperators[i],
             * i...
             */
            C1Operator( const std::vector< std::vector< Spacy::C1Operator > >& blockOperators,
                        const Spacy::VectorSpace& domain, const Spacy::VectorSpace& range );

            /// Compute \f$A(x)\f$.
            Spacy::Vector operator()( const Spacy::Vector& x ) const;

            /// Compute directional derivative \f$A'(x)\delta x\f$.
            Spacy::Vector d1( const Spacy::Vector& x, const Spacy::Vector& dx ) const;

            /// Get linearization \f$A
            LinearOperator linearization( const Spacy::Vector& x ) const;

            /// Set solver for linearizations.
            void setLinearSolver( std::function< Spacy::Vector( const Spacy::Vector& ) > solver );

            /// Access \f$A_{row,col}\f$.
            Spacy::C1Operator& operator()( size_type row, size_type col );

            /// Access \f$A_{row,col}\f$ (read-only).
            const Spacy::C1Operator& operator()( size_type row, size_type col ) const;

        private:
            std::vector< std::vector< Spacy::C1Operator > > blockOperators_;
            std::function< Spacy::Vector( const Spacy::Vector& ) > solver_;
        };
    }
}
