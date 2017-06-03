// This file was automatically generated using clang-type-erase.
// Please do not modify.

#pragma once

#include <Spacy/Util/storage.h>
#include <Spacy/Util/type_erasure_util.h>

#include <functional>
#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>
namespace Spacy
{
    namespace OperatorDetail
    {
        template < class Interface >
        struct Table
        {
            using call_const_Vector_ref_function =
                Vector ( * )( const clang::type_erasure::Storage&, const Vector& );
            call_const_Vector_ref_function call_const_Vector_ref;
            using domain_function = const VectorSpace& (*)( const clang::type_erasure::Storage& );
            domain_function domain;
            using range_function = const VectorSpace& (*)( const clang::type_erasure::Storage& );
            range_function range;
        };

        template < class Interface, class Impl >
        struct execution_wrapper
        {
            static Vector call_const_Vector_ref( const clang::type_erasure::Storage& data,
                                                 const Vector& x )
            {
                return data.template get< Impl >().operator()( x );
            }

            static const VectorSpace& domain( const clang::type_erasure::Storage& data )
            {
                return data.template get< Impl >().domain();
            }

            static const VectorSpace& range( const clang::type_erasure::Storage& data )
            {
                return data.template get< Impl >().range();
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
        using TryMemFn_domain = decltype( std::declval< T >().domain() );

        template < class T, class = void >
        struct HasMemFn_domain : std::false_type
        {
        };

        template < class T >
        struct HasMemFn_domain< T, type_erasure_table_detail::voider< TryMemFn_domain< T > > >
            : std::true_type
        {
        };

        template < class T >
        using TryMemFn_range = decltype( std::declval< T >().range() );

        template < class T, class = void >
        struct HasMemFn_range : std::false_type
        {
        };

        template < class T >
        struct HasMemFn_range< T, type_erasure_table_detail::voider< TryMemFn_range< T > > >
            : std::true_type
        {
        };

        template < class T >
        using ConceptImpl = type_erasure_table_detail::And<
            HasMemFn_call_const_Vector_ref<
                type_erasure_table_detail::remove_reference_wrapper_t< T > >,
            HasMemFn_domain< type_erasure_table_detail::remove_reference_wrapper_t< T > >,
            HasMemFn_range< type_erasure_table_detail::remove_reference_wrapper_t< T > > >;

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
