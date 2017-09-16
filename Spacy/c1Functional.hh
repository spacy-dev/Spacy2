// This file was automatically generated using clang-type-erase.
// Please do not modify.

#pragma once

#include <Spacy/Detail/c1Functional_table.h>
#include <Spacy/Util/storage.h>

#include <Spacy/Spaces/ScalarSpace/Real.hh>
#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>
namespace Spacy
{
    /// Type-erased differentiable functional \f$f:\ X \to \mathbb{R} \f$.
    class C1Functional
    {
    public:
        C1Functional() noexcept = default;

        template < class T,
                   typename std::enable_if< C1FunctionalDetail::Concept<
                       C1Functional, typename std::decay< T >::type >::value >::type* = nullptr >
        C1Functional( T&& value )
            : function_( {&C1FunctionalDetail::execution_wrapper<
                              C1Functional, type_erasure_table_detail::remove_reference_wrapper_t<
                                                std::decay_t< T > > >::call_const_Vector_ref,
                          &C1FunctionalDetail::execution_wrapper<
                              C1Functional, type_erasure_table_detail::remove_reference_wrapper_t<
                                                std::decay_t< T > > >::d1_const_Vector_ref,
                          &C1FunctionalDetail::execution_wrapper<
                              C1Functional, type_erasure_table_detail::remove_reference_wrapper_t<
                                                std::decay_t< T > > >::domain} ),
              impl_( std::forward< T >( value ) )
        {
        }

        template < class T,
                   typename std::enable_if< C1FunctionalDetail::Concept<
                       C1Functional, typename std::decay< T >::type >::value >::type* = nullptr >
        C1Functional& operator=( T&& value )
        {
            return *this = C1Functional( std::forward< T >( value ) );
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

        /// Compute derivative as function space element in \f$X^*\f$, where \f$x\in X\f$.
        Vector d1( const Vector& x ) const
        {
            assert( impl_ );
            return function_.d1_const_Vector_ref( impl_, x );
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
        C1FunctionalDetail::Table< C1Functional > function_;
        clang::type_erasure::SBOStorage< 16 > impl_;
    };
}
