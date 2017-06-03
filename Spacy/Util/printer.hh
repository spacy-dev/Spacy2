// This file was automatically generated using clang-type-erase.
// Please do not modify.

#pragma once

#include <Spacy/Util/Detail/printer_table.h>
#include <Spacy/Util/storage.h>

#include <functional>
#include <ostream>
namespace Spacy
{
    namespace Log
    {
        /// Interface for logger's printers.
        class Printer
        {
        public:
            Printer() noexcept = default;

            template < class T,
                       typename std::enable_if< PrinterDetail::Concept<
                           Printer, typename std::decay< T >::type >::value >::type* = nullptr >
            Printer( T&& value )
                : function_(
                      {&PrinterDetail::execution_wrapper<
                          Printer, type_erasure_table_detail::remove_reference_wrapper_t<
                                       std::decay_t< T > > >::
                           call_const_char_ptr_const_char_ptr_const_std_function_void_std_ostream_ref_ref} ),
                  impl_( std::forward< T >( value ) )
            {
            }

            template < class T,
                       typename std::enable_if< PrinterDetail::Concept<
                           Printer, typename std::decay< T >::type >::value >::type* = nullptr >
            Printer& operator=( T&& value )
            {
                return *this = Printer( std::forward< T >( value ) );
            }

            explicit operator bool() const noexcept
            {
                return bool( impl_ );
            }

            /// @brief Log printable according to its level and name.
            ///@param tag specifies the algorithm/function this log originates from
            ///@param name name of the logged value
            ///@param printable wraps writing of the logged value to some stream
            void operator()( const char* tag, const char* name,
                             const std::function< void( std::ostream& ) >& printable )
            {
                assert( impl_ );
                function_
                    .call_const_char_ptr_const_char_ptr_const_std_function_void_std_ostream_ref_ref(
                        impl_, tag, name, printable );
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
            PrinterDetail::Table< Printer > function_;
            clang::type_erasure::Storage impl_;
        };
    }
}
