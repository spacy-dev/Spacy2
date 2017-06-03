// This file was automatically generated using clang-type-erase.
// Please do not modify.

#pragma once

#include <Spacy/Util/storage.h>
#include <Spacy/Util/type_erasure_util.h>

#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>
namespace Spacy
{
    namespace ZeroVectorCreatorDetail
    {
        template < class Interface >
        struct Table
        {
            using call_const_VectorSpace_ptr_function =
                Vector ( * )( const clang::type_erasure::Storage&, const VectorSpace* );
            call_const_VectorSpace_ptr_function call_const_VectorSpace_ptr;
        };

        template < class Interface, class Impl >
        struct execution_wrapper
        {
            static Vector call_const_VectorSpace_ptr( const clang::type_erasure::Storage& data,
                                                      const VectorSpace* V )
            {
                return data.template get< Impl >().operator()( V );
            }
        };

        template < class T >
        using TryMemFn_call_const_VectorSpace_ptr =
            decltype( std::declval< T >().operator()( std::declval< const VectorSpace* >() ) );

        template < class T, class = void >
        struct HasMemFn_call_const_VectorSpace_ptr : std::false_type
        {
        };

        template < class T >
        struct HasMemFn_call_const_VectorSpace_ptr<
            T, type_erasure_table_detail::voider< TryMemFn_call_const_VectorSpace_ptr< T > > >
            : std::true_type
        {
        };

        template < class T >
        using ConceptImpl = type_erasure_table_detail::And< HasMemFn_call_const_VectorSpace_ptr<
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
