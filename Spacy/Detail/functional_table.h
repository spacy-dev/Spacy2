// This file was automatically generated using clang-type-erase.
// Please do not modify.

#pragma once

#include <Spacy/Util/storage.h>
#include <Spacy/Util/type_erasure_util.h>

#include <Spacy/Spaces/ScalarSpace/Real.hh>
#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>
namespace Spacy
{
    namespace FunctionalDetail
    {
        template < class Interface >
        struct Table
        {
            using call_const_Vector_ref_function =
                Real ( * )( const clang::type_erasure::SBOStorage< 16 >&, const Vector& );
            call_const_Vector_ref_function call_const_Vector_ref;
            using domain_function =
                const VectorSpace& (*)( const clang::type_erasure::SBOStorage< 16 >& );
            domain_function domain;
        };

        template < class Interface, class Impl >
        struct execution_wrapper
        {
            static Real call_const_Vector_ref( const clang::type_erasure::SBOStorage< 16 >& data,
                                               const Vector& x )
            {
                return data.template get< Impl >().operator()( x );
            }

            static const VectorSpace& domain( const clang::type_erasure::SBOStorage< 16 >& data )
            {
                return data.template get< Impl >().domain();
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
        using ConceptImpl = type_erasure_table_detail::And<
            HasMemFn_call_const_Vector_ref<
                type_erasure_table_detail::remove_reference_wrapper_t< T > >,
            HasMemFn_domain< type_erasure_table_detail::remove_reference_wrapper_t< T > > >;

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
