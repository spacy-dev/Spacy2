// This file was automatically generated using clang-type-erase.
// Please do not modify.

#pragma once

#include <Spacy/Detail/functional_table.h>
#include <Spacy/Util/storage.h>

#include <Spacy/Spaces/ScalarSpace/Real.hh>
#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>
namespace Spacy
{
    /// Type-erased functional \f$f:\ X \to \mathbb{R} \f$.
    class Functional
    {
    public:
        Functional() noexcept = default;

        template < class T,
                   typename std::enable_if< FunctionalDetail::Concept<
                       Functional, typename std::decay< T >::type >::value >::type* = nullptr >
        Functional( T&& value )
            : function_( {&FunctionalDetail::execution_wrapper<
                              Functional, type_erasure_table_detail::remove_reference_wrapper_t<
                                              std::decay_t< T > > >::call_const_Vector_ref,
                          &FunctionalDetail::execution_wrapper<
                              Functional, type_erasure_table_detail::remove_reference_wrapper_t<
                                              std::decay_t< T > > >::domain} ),
              impl_( std::forward< T >( value ) )
        {
        }

        template < class T,
                   typename std::enable_if< FunctionalDetail::Concept<
                       Functional, typename std::decay< T >::type >::value >::type* = nullptr >
        Functional& operator=( T&& value )
        {
            return *this = Functional( std::forward< T >( value ) );
        }

        explicit operator bool() const noexcept
        {
            return bool( impl_ );
        }

        /// Apply functional.
        Real operator()( const Vector& x ) const
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
        FunctionalDetail::Table< Functional > function_;
        clang::type_erasure::Storage impl_;
    };
}
