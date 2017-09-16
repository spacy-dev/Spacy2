// This file was automatically generated using clang-type-erase.
// Please do not modify.

#pragma once

#include <Spacy/Util/storage.h>
#include <Spacy/Util/type_erasure_util.h>

#include <functional>
#include <Spacy/Spaces/ScalarSpace/Real.hh>
#include <Spacy/Util/Exceptions/invalidArgumentException.hh>
#include <Spacy/linearSolver.hh>
#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>
namespace Spacy
{
    namespace LinearOperatorDetail
    {
        template < class Interface >
        struct Table
        {
            using call_const_Vector_ref_function =
                Vector ( * )( const clang::type_erasure::SBOStorage< 16 >&, const Vector& );
            call_const_Vector_ref_function call_const_Vector_ref;
            using call_const_LinearOperator_ref_function =
                Real ( * )( const clang::type_erasure::SBOStorage< 16 >&,
                            const clang::type_erasure::SBOStorage< 16 >& );
            call_const_LinearOperator_ref_function call_const_LinearOperator_ref;
            using add_const_LinearOperator_ref_function =
                Interface& (*)( Interface&, clang::type_erasure::SBOStorage< 16 >&,
                                const clang::type_erasure::SBOStorage< 16 >& );
            add_const_LinearOperator_ref_function add_const_LinearOperator_ref;
            using subtract_const_LinearOperator_ref_function =
                Interface& (*)( Interface&, clang::type_erasure::SBOStorage< 16 >&,
                                const clang::type_erasure::SBOStorage< 16 >& );
            subtract_const_LinearOperator_ref_function subtract_const_LinearOperator_ref;
            using multiply_double_function = Interface& (*)( Interface&,
                                                             clang::type_erasure::SBOStorage< 16 >&,
                                                             double );
            multiply_double_function multiply_double;
            using negate_function = Interface ( * )( const clang::type_erasure::SBOStorage< 16 >& );
            negate_function negate;
            using compare_const_LinearOperator_ref_function =
                bool ( * )( const clang::type_erasure::SBOStorage< 16 >&,
                            const clang::type_erasure::SBOStorage< 16 >& );
            compare_const_LinearOperator_ref_function compare_const_LinearOperator_ref;
            using solver_function = std::function< Vector( const Vector& ) > ( * )(
                const clang::type_erasure::SBOStorage< 16 >& );
            solver_function solver;
            using domain_function =
                const VectorSpace& (*)( const clang::type_erasure::SBOStorage< 16 >& );
            domain_function domain;
            using range_function =
                const VectorSpace& (*)( const clang::type_erasure::SBOStorage< 16 >& );
            range_function range;
            using space_function =
                const VectorSpace& (*)( const clang::type_erasure::SBOStorage< 16 >& );
            space_function space;
        };

        template < class Interface, class Impl >
        struct execution_wrapper
        {
            static Vector call_const_Vector_ref( const clang::type_erasure::SBOStorage< 16 >& data,
                                                 const Vector& x )
            {
                return data.template get< Impl >().operator()( x );
            }

            static Real
            call_const_LinearOperator_ref( const clang::type_erasure::SBOStorage< 16 >& data,
                                           const clang::type_erasure::SBOStorage< 16 >& x )
            {
                return data.template get< Impl >().operator()( x.template get< Impl >() );
            }

            static Interface&
            add_const_LinearOperator_ref( Interface& interface,
                                          clang::type_erasure::SBOStorage< 16 >& data,
                                          const clang::type_erasure::SBOStorage< 16 >& y )
            {
                data.template get< Impl >().operator+=( y.template get< Impl >() );
                return interface;
            }

            static Interface&
            subtract_const_LinearOperator_ref( Interface& interface,
                                               clang::type_erasure::SBOStorage< 16 >& data,
                                               const clang::type_erasure::SBOStorage< 16 >& y )
            {
                data.template get< Impl >().operator-=( y.template get< Impl >() );
                return interface;
            }

            static Interface& multiply_double( Interface& interface,
                                               clang::type_erasure::SBOStorage< 16 >& data,
                                               double a )
            {
                data.template get< Impl >().operator*=( std::move( a ) );
                return interface;
            }

            static Interface negate( const clang::type_erasure::SBOStorage< 16 >& data )
            {
                return data.template get< Impl >().operator-();
            }

            static bool
            compare_const_LinearOperator_ref( const clang::type_erasure::SBOStorage< 16 >& data,
                                              const clang::type_erasure::SBOStorage< 16 >& y )
            {
                return data.template get< Impl >().operator==( y.template get< Impl >() );
            }

            static std::function< Vector( const Vector& ) >
            solver( const clang::type_erasure::SBOStorage< 16 >& data )
            {
                return data.template get< Impl >().solver();
            }

            static const VectorSpace& domain( const clang::type_erasure::SBOStorage< 16 >& data )
            {
                return data.template get< Impl >().domain();
            }

            static const VectorSpace& range( const clang::type_erasure::SBOStorage< 16 >& data )
            {
                return data.template get< Impl >().range();
            }

            static const VectorSpace& space( const clang::type_erasure::SBOStorage< 16 >& data )
            {
                return data.template get< Impl >().space();
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
        using TryMemFn_call_const_LinearOperator_ref =
            decltype( std::declval< T >().operator()( std::declval< const T& >() ) );

        template < class T, class = void >
        struct HasMemFn_call_const_LinearOperator_ref : std::false_type
        {
        };

        template < class T >
        struct HasMemFn_call_const_LinearOperator_ref<
            T, type_erasure_table_detail::voider< TryMemFn_call_const_LinearOperator_ref< T > > >
            : std::true_type
        {
        };

        template < class T >
        using TryMemFn_add_const_LinearOperator_ref =
            decltype( std::declval< T >().operator+=( std::declval< const T& >() ) );

        template < class T, class = void >
        struct HasMemFn_add_const_LinearOperator_ref : std::false_type
        {
        };

        template < class T >
        struct HasMemFn_add_const_LinearOperator_ref<
            T, type_erasure_table_detail::voider< TryMemFn_add_const_LinearOperator_ref< T > > >
            : std::true_type
        {
        };

        template < class T >
        using TryMemFn_subtract_const_LinearOperator_ref =
            decltype( std::declval< T >().operator-=( std::declval< const T& >() ) );

        template < class T, class = void >
        struct HasMemFn_subtract_const_LinearOperator_ref : std::false_type
        {
        };

        template < class T >
        struct HasMemFn_subtract_const_LinearOperator_ref<
            T,
            type_erasure_table_detail::voider< TryMemFn_subtract_const_LinearOperator_ref< T > > >
            : std::true_type
        {
        };

        template < class T >
        using TryMemFn_multiply_double =
            decltype( std::declval< T >().operator*=( std::declval< double >() ) );

        template < class T, class = void >
        struct HasMemFn_multiply_double : std::false_type
        {
        };

        template < class T >
        struct HasMemFn_multiply_double<
            T, type_erasure_table_detail::voider< TryMemFn_multiply_double< T > > > : std::true_type
        {
        };

        template < class T >
        using TryMemFn_negate = decltype( std::declval< T >().operator-() );

        template < class T, class = void >
        struct HasMemFn_negate : std::false_type
        {
        };

        template < class T >
        struct HasMemFn_negate< T, type_erasure_table_detail::voider< TryMemFn_negate< T > > >
            : std::true_type
        {
        };

        template < class T >
        using TryMemFn_compare_const_LinearOperator_ref =
            decltype( std::declval< T >().operator==( std::declval< const T& >() ) );

        template < class T, class = void >
        struct HasMemFn_compare_const_LinearOperator_ref : std::false_type
        {
        };

        template < class T >
        struct HasMemFn_compare_const_LinearOperator_ref<
            T, type_erasure_table_detail::voider< TryMemFn_compare_const_LinearOperator_ref< T > > >
            : std::true_type
        {
        };

        template < class T >
        using TryMemFn_solver = decltype( std::declval< T >().solver() );

        template < class T, class = void >
        struct HasMemFn_solver : std::false_type
        {
        };

        template < class T >
        struct HasMemFn_solver< T, type_erasure_table_detail::voider< TryMemFn_solver< T > > >
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
        using TryMemFn_space = decltype( std::declval< T >().space() );

        template < class T, class = void >
        struct HasMemFn_space : std::false_type
        {
        };

        template < class T >
        struct HasMemFn_space< T, type_erasure_table_detail::voider< TryMemFn_space< T > > >
            : std::true_type
        {
        };

        template < class T >
        using ConceptImpl = type_erasure_table_detail::And<
            HasMemFn_call_const_Vector_ref<
                type_erasure_table_detail::remove_reference_wrapper_t< T > >,
            HasMemFn_call_const_LinearOperator_ref<
                type_erasure_table_detail::remove_reference_wrapper_t< T > >,
            HasMemFn_add_const_LinearOperator_ref<
                type_erasure_table_detail::remove_reference_wrapper_t< T > >,
            HasMemFn_subtract_const_LinearOperator_ref<
                type_erasure_table_detail::remove_reference_wrapper_t< T > >,
            HasMemFn_multiply_double< type_erasure_table_detail::remove_reference_wrapper_t< T > >,
            HasMemFn_negate< type_erasure_table_detail::remove_reference_wrapper_t< T > >,
            HasMemFn_compare_const_LinearOperator_ref<
                type_erasure_table_detail::remove_reference_wrapper_t< T > >,
            HasMemFn_solver< type_erasure_table_detail::remove_reference_wrapper_t< T > >,
            HasMemFn_domain< type_erasure_table_detail::remove_reference_wrapper_t< T > >,
            HasMemFn_range< type_erasure_table_detail::remove_reference_wrapper_t< T > >,
            HasMemFn_space< type_erasure_table_detail::remove_reference_wrapper_t< T > > >;

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
