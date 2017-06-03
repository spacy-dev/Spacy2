// This file was automatically generated using clang-type-erase.
// Please do not modify.

#pragma once

#include <Spacy/Util/storage.h>
#include <Spacy/Util/type_erasure_util.h>

#include <functional>
#include <ostream>
namespace Spacy
{
    namespace Log
    {
        namespace PrinterDetail
        {
            template < class Interface >
            struct Table
            {
                using call_const_char_ptr_const_char_ptr_const_std_function_void_std_ostream_ref_ref_function =
                    void ( * )( clang::type_erasure::Storage&, const char*, const char*,
                                const std::function< void( std::ostream& ) >& );
                call_const_char_ptr_const_char_ptr_const_std_function_void_std_ostream_ref_ref_function
                    call_const_char_ptr_const_char_ptr_const_std_function_void_std_ostream_ref_ref;
            };

            template < class Interface, class Impl >
            struct execution_wrapper
            {
                static void
                call_const_char_ptr_const_char_ptr_const_std_function_void_std_ostream_ref_ref(
                    clang::type_erasure::Storage& data, const char* tag, const char* name,
                    const std::function< void( std::ostream& ) >& printable )
                {
                    data.template get< Impl >().operator()( tag, name, printable );
                }
            };

            template < class T >
            using TryMemFn_call_const_char_ptr_const_char_ptr_const_std_function_void_std_ostream_ref_ref =
                decltype( std::declval< T >().operator()(
                    std::declval< const char* >(), std::declval< const char* >(),
                    std::declval< const std::function< void( std::ostream& ) >& >() ) );

            template < class T, class = void >
            struct
                HasMemFn_call_const_char_ptr_const_char_ptr_const_std_function_void_std_ostream_ref_ref
                : std::false_type
            {
            };

            template < class T >
            struct HasMemFn_call_const_char_ptr_const_char_ptr_const_std_function_void_std_ostream_ref_ref<
                T,
                type_erasure_table_detail::voider<
                    TryMemFn_call_const_char_ptr_const_char_ptr_const_std_function_void_std_ostream_ref_ref<
                        T > > > : std::true_type
            {
            };

            template < class T >
            using ConceptImpl = type_erasure_table_detail::And<
                HasMemFn_call_const_char_ptr_const_char_ptr_const_std_function_void_std_ostream_ref_ref<
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
}
