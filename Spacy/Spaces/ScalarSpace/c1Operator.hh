#pragma once

#include <memory>
#include <functional>

#include <Spacy/Util/Base/OperatorBase.hh>

namespace Spacy
{
    /// @cond
    class LinearOperator;
    class Real;
    class Vector;
    /// @endcond

    /** @addtogroup ScalarGroup @{ */
    namespace Scalar
    {
        /// @cond
        class LinearOperator;
        /// @endcond

        class C1Operator : public Spacy::OperatorBase
        {
        public:
            C1Operator( std::function< double( double ) > value,
                        std::function< double( double ) > derivative );

            C1Operator( std::function< double( double ) > value,
                        std::function< double( double ) > derivative, const VectorSpace& domain,
                        const VectorSpace& range );

            Spacy::Vector operator()( const ::Spacy::Vector& x ) const;

            Spacy::Vector d1( const ::Spacy::Vector& x, const ::Spacy::Vector& dx ) const;

            LinearOperator linearization( const ::Spacy::Vector& x ) const;

        private:
            std::function< double( double ) > value_;
            std::function< double( double ) > derivative_;
            std::shared_ptr< VectorSpace > operatorSpace_ = nullptr;
        };
    }
    /** @} */
}
