// This file was automatically generated using clang-type-erase.
// Please do not modify.

#pragma once

#include <Spacy/Util/storage.h>
#include <Spacy/Util/type_erasure_util.h>

#include <functional>
#include <Spacy/vector.hh>
namespace Spacy
{
    namespace IndefiniteLinearSolverDetail
    {
        template < class Interface >
        struct Table
        {
            using call_const_Vector_ref_function =
                Vector ( * )( const clang::type_erasure::Storage&, const Vector& );
            call_const_Vector_ref_function call_const_Vector_ref;
            using isPositiveDefinite_function = bool ( * )( const clang::type_erasure::Storage& );
            isPositiveDefinite_function isPositiveDefinite;
        };

        template < class Interface, class Impl >
        struct execution_wrapper
        {
            static Vector call_const_Vector_ref( const clang::type_erasure::Storage& data,
                                                 const Vector& x )
            {
                return data.template get< Impl >().operator()( x );
            }

            static bool isPositiveDefinite( const clang::type_erasure::Storage& data )
            {
                return data.template get< Impl >().isPositiveDefinite();
            }
        };

        template < class T >
        using TryMemFn_call_const_Vector_ref =
            decltype( std::declval< T >().operator()( std::declval< const Vector& >() ) );

        template < class T, class = void >
        struct HasMemFn_call_const_Vector_ref : std::false_type
        {
        };

        template < class T >
        struct HasMemFn_call_const_Vector_ref<
            T, type_erasure_table_detail::voider< TryMemFn_call_const_Vector_ref< T > > >
            : std::true_type
        {
        };

        template < class T >
        using TryMemFn_isPositiveDefinite = decltype( std::declval< T >().isPositiveDefinite() );

        template < class T, class = void >
        struct HasMemFn_isPositiveDefinite : std::false_type
        {
        };

        template < class T >
        struct HasMemFn_isPositiveDefinite<
            T, type_erasure_table_detail::voider< TryMemFn_isPositiveDefinite< T > > >
            : std::true_type
        {
        };

        template < class T >
        using ConceptImpl = type_erasure_table_detail::And<
            HasMemFn_call_const_Vector_ref<
                type_erasure_table_detail::remove_reference_wrapper_t< T > >,
            HasMemFn_isPositiveDefinite<
                type_erasure_table_detail::remove_reference_wrapper_t< T > > >;

        template < class Impl, class T, bool = std::is_base_of< Impl, T >::value >
        struct Concept : std::false_type
        {
        };

        template < class Impl, class T >
        struct Concept< Impl, T, false > : ConceptImpl< T >
        {
        };
    }
}
