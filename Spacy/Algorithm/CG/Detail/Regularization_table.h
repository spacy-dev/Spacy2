// This file was automatically generated using clang-type-erase.
// Please do not modify.

#pragma once

#include <Spacy/Util/storage.h>
#include <Spacy/Util/type_erasure_util.h>

#include <Spacy/Spaces/ScalarSpace/Real.hh>
#include <Spacy/vector.hh>
namespace Spacy
{
    namespace CG
    {
        namespace RegularizationDetail
        {
            template < class Interface >
            struct Table
            {
                using init_function = void ( * )( clang::type_erasure::Storage& );
                init_function init;
                using apply_Real_ref_Real_function =
                    void ( * )( const clang::type_erasure::Storage&, Real&, Real );
                apply_Real_ref_Real_function apply_Real_ref_Real;
                using update_Real_Real_function = void ( * )( clang::type_erasure::Storage&, Real,
                                                              Real );
                update_Real_Real_function update_Real_Real;
                using adjustResidual_Real_const_Vector_ref_Vector_ref_function =
                    void ( * )( const clang::type_erasure::Storage&, Real, const Vector&, Vector& );
                adjustResidual_Real_const_Vector_ref_Vector_ref_function
                    adjustResidual_Real_const_Vector_ref_Vector_ref;
            };

            template < class Interface, class Impl >
            struct execution_wrapper
            {
                static void init( clang::type_erasure::Storage& data )
                {
                    data.template get< Impl >().init();
                }

                static void apply_Real_ref_Real( const clang::type_erasure::Storage& data,
                                                 Real& qAq, Real qPq )
                {
                    data.template get< Impl >().apply( qAq, std::move( qPq ) );
                }

                static void update_Real_Real( clang::type_erasure::Storage& data, Real qAq,
                                              Real qPq )
                {
                    data.template get< Impl >().update( std::move( qAq ), std::move( qPq ) );
                }

                static void adjustResidual_Real_const_Vector_ref_Vector_ref(
                    const clang::type_erasure::Storage& data, Real alpha, const Vector& Pq,
                    Vector& r )
                {
                    data.template get< Impl >().adjustResidual( std::move( alpha ), Pq, r );
                }
            };

            template < class T >
            using TryMemFn_init = decltype( std::declval< T >().init() );

            template < class T, class = void >
            struct HasMemFn_init : std::false_type
            {
            };

            template < class T >
            struct HasMemFn_init< T, type_erasure_table_detail::voider< TryMemFn_init< T > > >
                : std::true_type
            {
            };

            template < class T >
            using TryMemFn_apply_Real_ref_Real = decltype(
                std::declval< T >().apply( std::declval< Real& >(), std::declval< Real >() ) );

            template < class T, class = void >
            struct HasMemFn_apply_Real_ref_Real : std::false_type
            {
            };

            template < class T >
            struct HasMemFn_apply_Real_ref_Real<
                T, type_erasure_table_detail::voider< TryMemFn_apply_Real_ref_Real< T > > >
                : std::true_type
            {
            };

            template < class T >
            using TryMemFn_update_Real_Real = decltype(
                std::declval< T >().update( std::declval< Real >(), std::declval< Real >() ) );

            template < class T, class = void >
            struct HasMemFn_update_Real_Real : std::false_type
            {
            };

            template < class T >
            struct HasMemFn_update_Real_Real<
                T, type_erasure_table_detail::voider< TryMemFn_update_Real_Real< T > > >
                : std::true_type
            {
            };

            template < class T >
            using TryMemFn_adjustResidual_Real_const_Vector_ref_Vector_ref =
                decltype( std::declval< T >().adjustResidual( std::declval< Real >(),
                                                              std::declval< const Vector& >(),
                                                              std::declval< Vector& >() ) );

            template < class T, class = void >
            struct HasMemFn_adjustResidual_Real_const_Vector_ref_Vector_ref : std::false_type
            {
            };

            template < class T >
            struct HasMemFn_adjustResidual_Real_const_Vector_ref_Vector_ref<
                T, type_erasure_table_detail::voider<
                       TryMemFn_adjustResidual_Real_const_Vector_ref_Vector_ref< T > > >
                : std::true_type
            {
            };

            template < class T >
            using ConceptImpl = type_erasure_table_detail::And<
                HasMemFn_init< type_erasure_table_detail::remove_reference_wrapper_t< T > >,
                HasMemFn_apply_Real_ref_Real<
                    type_erasure_table_detail::remove_reference_wrapper_t< T > >,
                HasMemFn_update_Real_Real<
                    type_erasure_table_detail::remove_reference_wrapper_t< T > >,
                HasMemFn_adjustResidual_Real_const_Vector_ref_Vector_ref<
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
