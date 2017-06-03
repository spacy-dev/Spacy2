// This file was automatically generated using clang-type-erase.
// Please do not modify.

#pragma once

#include <Spacy/Detail/operator_table.h>
#include <Spacy/Util/storage.h>

#include <functional>
#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>
namespace Spacy
{
    /// An operator that does not know about domain and range spaces.
    /// Good enough for some algorithms (i.e. for Krylov-methods).
    using CallableOperator = std::function< Vector( const Vector& ) >;
    /// Type-erased operator \f$A:\ X \to Y \f$.
    class Operator
    {
    public:
        Operator() noexcept = default;

        template < class T,
                   typename std::enable_if< OperatorDetail::Concept<
                       Operator, typename std::decay< T >::type >::value >::type* = nullptr >
        Operator( T&& value )
            : function_( {&OperatorDetail::execution_wrapper<
                              Operator, type_erasure_table_detail::remove_reference_wrapper_t<
                                            std::decay_t< T > > >::call_const_Vector_ref,
                          &OperatorDetail::execution_wrapper<
                              Operator, type_erasure_table_detail::remove_reference_wrapper_t<
                                            std::decay_t< T > > >::domain,
                          &OperatorDetail::execution_wrapper<
                              Operator, type_erasure_table_detail::remove_reference_wrapper_t<
                                            std::decay_t< T > > >::range} ),
              impl_( std::forward< T >( value ) )
        {
        }

        template < class T,
                   typename std::enable_if< OperatorDetail::Concept<
                       Operator, typename std::decay< T >::type >::value >::type* = nullptr >
        Operator& operator=( T&& value )
        {
            return *this = Operator( std::forward< T >( value ) );
        }

        explicit operator bool() const noexcept
        {
            return bool( impl_ );
        }

        /// Apply operator.
        Vector operator()( const Vector& x ) const
        {
            assert( impl_ );
            return function_.call_const_Vector_ref( impl_, x );
        }

        /// Access domain space \f$X\f$.
        const VectorSpace& domain() const
        {
            assert( impl_ );
            return function_.domain( impl_ );
        }

        /// Access range space \f$Y\f$.
        const VectorSpace& range() const
        {
            assert( impl_ );
            return function_.range( impl_ );
        }

        template < class T >
        T* target() noexcept
        {
            return impl_.template target< T >();
        }

        template < class T >
        const T* target() const noexcept
        {
            return impl_.template target< T >();
        }

    private:
        OperatorDetail::Table< Operator > function_;
        clang::type_erasure::Storage impl_;
    };
}
