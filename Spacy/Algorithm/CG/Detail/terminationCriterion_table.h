// This file was automatically generated using clang-type-erase.
// Please do not modify.

#pragma once

#include <Spacy/Util/storage.h>
#include <Spacy/Util/type_erasure_util.h>

namespace Spacy
{
    namespace CG
    {
        namespace TerminationCriterionDetail
        {
            template < class Interface >
            struct Table
            {
                using call_function = bool ( * )( const clang::type_erasure::Storage& );
                call_function call;
                using clear_function = void ( * )( clang::type_erasure::Storage& );
                clear_function clear;
                using update_double_double_double_double_function =
                    void ( * )( clang::type_erasure::Storage&, double, double, double, double );
                update_double_double_double_double_function update_double_double_double_double;
                using vanishingStep_function = bool ( * )( const clang::type_erasure::Storage& );
                vanishingStep_function vanishingStep;
                using minimalDecreaseAchieved_function =
                    bool ( * )( const clang::type_erasure::Storage& );
                minimalDecreaseAchieved_function minimalDecreaseAchieved;
                using set_eps_double_function = void ( * )( clang::type_erasure::Storage&, double );
                set_eps_double_function set_eps_double;
                using setAbsoluteAccuracy_double_function =
                    void ( * )( clang::type_erasure::Storage&, double );
                setAbsoluteAccuracy_double_function setAbsoluteAccuracy_double;
                using setMinimalAccuracy_double_function =
                    void ( * )( clang::type_erasure::Storage&, double );
                setMinimalAccuracy_double_function setMinimalAccuracy_double;
                using setRelativeAccuracy_double_function =
                    void ( * )( clang::type_erasure::Storage&, double );
                setRelativeAccuracy_double_function setRelativeAccuracy_double;
            };

            template < class Interface, class Impl >
            struct execution_wrapper
            {
                static bool call( const clang::type_erasure::Storage& data )
                {
                    return data.template get< Impl >().operator()();
                }

                static void clear( clang::type_erasure::Storage& data )
                {
                    data.template get< Impl >().clear();
                }

                static void update_double_double_double_double( clang::type_erasure::Storage& data,
                                                                double alpha, double qAq,
                                                                double qPq, double rPINVr )
                {
                    data.template get< Impl >().update( std::move( alpha ), std::move( qAq ),
                                                        std::move( qPq ), std::move( rPINVr ) );
                }

                static bool vanishingStep( const clang::type_erasure::Storage& data )
                {
                    return data.template get< Impl >().vanishingStep();
                }

                static bool minimalDecreaseAchieved( const clang::type_erasure::Storage& data )
                {
                    return data.template get< Impl >().minimalDecreaseAchieved();
                }

                static void set_eps_double( clang::type_erasure::Storage& data, double eps )
                {
                    data.template get< Impl >().set_eps( std::move( eps ) );
                }

                static void setAbsoluteAccuracy_double( clang::type_erasure::Storage& data,
                                                        double accuracy )
                {
                    data.template get< Impl >().setAbsoluteAccuracy( std::move( accuracy ) );
                }

                static void setMinimalAccuracy_double( clang::type_erasure::Storage& data,
                                                       double accuracy )
                {
                    data.template get< Impl >().setMinimalAccuracy( std::move( accuracy ) );
                }

                static void setRelativeAccuracy_double( clang::type_erasure::Storage& data,
                                                        double accuracy )
                {
                    data.template get< Impl >().setRelativeAccuracy( std::move( accuracy ) );
                }
            };

            template < class T >
            using TryMemFn_call = decltype( std::declval< T >().operator()() );

            template < class T, class = void >
            struct HasMemFn_call : std::false_type
            {
            };

            template < class T >
            struct HasMemFn_call< T, type_erasure_table_detail::voider< TryMemFn_call< T > > >
                : std::true_type
            {
            };

            template < class T >
            using TryMemFn_clear = decltype( std::declval< T >().clear() );

            template < class T, class = void >
            struct HasMemFn_clear : std::false_type
            {
            };

            template < class T >
            struct HasMemFn_clear< T, type_erasure_table_detail::voider< TryMemFn_clear< T > > >
                : std::true_type
            {
            };

            template < class T >
            using TryMemFn_update_double_double_double_double = decltype(
                std::declval< T >().update( std::declval< double >(), std::declval< double >(),
                                            std::declval< double >(), std::declval< double >() ) );

            template < class T, class = void >
            struct HasMemFn_update_double_double_double_double : std::false_type
            {
            };

            template < class T >
            struct HasMemFn_update_double_double_double_double<
                T, type_erasure_table_detail::voider<
                       TryMemFn_update_double_double_double_double< T > > > : std::true_type
            {
            };

            template < class T >
            using TryMemFn_vanishingStep = decltype( std::declval< T >().vanishingStep() );

            template < class T, class = void >
            struct HasMemFn_vanishingStep : std::false_type
            {
            };

            template < class T >
            struct HasMemFn_vanishingStep<
                T, type_erasure_table_detail::voider< TryMemFn_vanishingStep< T > > >
                : std::true_type
            {
            };

            template < class T >
            using TryMemFn_minimalDecreaseAchieved =
                decltype( std::declval< T >().minimalDecreaseAchieved() );

            template < class T, class = void >
            struct HasMemFn_minimalDecreaseAchieved : std::false_type
            {
            };

            template < class T >
            struct HasMemFn_minimalDecreaseAchieved<
                T, type_erasure_table_detail::voider< TryMemFn_minimalDecreaseAchieved< T > > >
                : std::true_type
            {
            };

            template < class T >
            using TryMemFn_set_eps_double =
                decltype( std::declval< T >().set_eps( std::declval< double >() ) );

            template < class T, class = void >
            struct HasMemFn_set_eps_double : std::false_type
            {
            };

            template < class T >
            struct HasMemFn_set_eps_double<
                T, type_erasure_table_detail::voider< TryMemFn_set_eps_double< T > > >
                : std::true_type
            {
            };

            template < class T >
            using TryMemFn_setAbsoluteAccuracy_double =
                decltype( std::declval< T >().setAbsoluteAccuracy( std::declval< double >() ) );

            template < class T, class = void >
            struct HasMemFn_setAbsoluteAccuracy_double : std::false_type
            {
            };

            template < class T >
            struct HasMemFn_setAbsoluteAccuracy_double<
                T, type_erasure_table_detail::voider< TryMemFn_setAbsoluteAccuracy_double< T > > >
                : std::true_type
            {
            };

            template < class T >
            using TryMemFn_setMinimalAccuracy_double =
                decltype( std::declval< T >().setMinimalAccuracy( std::declval< double >() ) );

            template < class T, class = void >
            struct HasMemFn_setMinimalAccuracy_double : std::false_type
            {
            };

            template < class T >
            struct HasMemFn_setMinimalAccuracy_double<
                T, type_erasure_table_detail::voider< TryMemFn_setMinimalAccuracy_double< T > > >
                : std::true_type
            {
            };

            template < class T >
            using TryMemFn_setRelativeAccuracy_double =
                decltype( std::declval< T >().setRelativeAccuracy( std::declval< double >() ) );

            template < class T, class = void >
            struct HasMemFn_setRelativeAccuracy_double : std::false_type
            {
            };

            template < class T >
            struct HasMemFn_setRelativeAccuracy_double<
                T, type_erasure_table_detail::voider< TryMemFn_setRelativeAccuracy_double< T > > >
                : std::true_type
            {
            };

            template < class T >
            using ConceptImpl = type_erasure_table_detail::And<
                HasMemFn_call< type_erasure_table_detail::remove_reference_wrapper_t< T > >,
                HasMemFn_clear< type_erasure_table_detail::remove_reference_wrapper_t< T > >,
                HasMemFn_update_double_double_double_double<
                    type_erasure_table_detail::remove_reference_wrapper_t< T > >,
                HasMemFn_vanishingStep<
                    type_erasure_table_detail::remove_reference_wrapper_t< T > >,
                HasMemFn_minimalDecreaseAchieved<
                    type_erasure_table_detail::remove_reference_wrapper_t< T > >,
                HasMemFn_set_eps_double<
                    type_erasure_table_detail::remove_reference_wrapper_t< T > >,
                HasMemFn_setAbsoluteAccuracy_double<
                    type_erasure_table_detail::remove_reference_wrapper_t< T > >,
                HasMemFn_setMinimalAccuracy_double<
                    type_erasure_table_detail::remove_reference_wrapper_t< T > >,
                HasMemFn_setRelativeAccuracy_double<
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
