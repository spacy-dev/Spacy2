// This file was automatically generated using clang-type-erase.
// Please do not modify.

#pragma once

#include <Spacy/Detail/dynamicOperator_table.h>
#include <Spacy/Util/storage.h>

#include <functional>
#include <Spacy/linearOperator.hh>
#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>
namespace Spacy
{
    /// A time-dependent operator that does not know about domain and range spaces.
    using DynamicCallableOperator = std::function< Vector( double, const Vector& ) >;
    /// Type-erased time-dependent operator \f$A:\ [0,T] \times X \to Y \f$.
    class DynamicOperator
    {
    public:
        DynamicOperator() noexcept = default;

        template < class T,
                   typename std::enable_if< DynamicOperatorDetail::Concept<
                       DynamicOperator, typename std::decay< T >::type >::value >::type* = nullptr >
        DynamicOperator( T&& value )
            : function_(
                  {&DynamicOperatorDetail::execution_wrapper<
                       DynamicOperator, type_erasure_table_detail::remove_reference_wrapper_t<
                                            std::decay_t< T > > >::call_const_Vector_ref,
                   &DynamicOperatorDetail::execution_wrapper<
                       DynamicOperator, type_erasure_table_detail::remove_reference_wrapper_t<
                                            std::decay_t< T > > >::M,
                   &DynamicOperatorDetail::execution_wrapper<
                       DynamicOperator, type_erasure_table_detail::remove_reference_wrapper_t<
                                            std::decay_t< T > > >::domain,
                   &DynamicOperatorDetail::execution_wrapper<
                       DynamicOperator, type_erasure_table_detail::remove_reference_wrapper_t<
                                            std::decay_t< T > > >::range} ),
              impl_( std::forward< T >( value ) )
        {
        }

        template < class T,
                   typename std::enable_if< DynamicOperatorDetail::Concept<
                       DynamicOperator, typename std::decay< T >::type >::value >::type* = nullptr >
        DynamicOperator& operator=( T&& value )
        {
            return *this = DynamicOperator( std::forward< T >( value ) );
        }

        explicit operator bool() const noexcept
        {
            return bool( impl_ );
        }

        /// Apply operator.
        Vector operator()( const Vector& x ) const
        {
            assert( impl_ );
            return function_.call_const_Vector_ref( impl_, x );
        }

        LinearOperator M() const
        {
            assert( impl_ );
            return function_.M( impl_ );
        }

        /// Access domain space \f$X\f$.
        const VectorSpace& domain() const
        {
            assert( impl_ );
            return function_.domain( impl_ );
        }

        /// Access range space \f$Y\f$.
        const VectorSpace& range() const
        {
            assert( impl_ );
            return function_.range( impl_ );
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
        DynamicOperatorDetail::Table< DynamicOperator > function_;
        clang::type_erasure::SBOStorage< 16 > impl_;
    };
    /// Type-erased time-dependent linear operator \f$A:\ [0,T] \times X \to Y \f$.
    class DynamicLinearOperator
    {
    public:
        DynamicLinearOperator() noexcept = default;

        template <
            class T,
            typename std::enable_if< DynamicLinearOperatorDetail::Concept<
                DynamicLinearOperator, typename std::decay< T >::type >::value >::type* = nullptr >
        DynamicLinearOperator( T&& value )
            : function_(
                  {&DynamicLinearOperatorDetail::execution_wrapper<
                       DynamicLinearOperator,
                       type_erasure_table_detail::remove_reference_wrapper_t<
                           std::decay_t< T > > >::call_double_const_Vector_ref,
                   &DynamicLinearOperatorDetail::execution_wrapper<
                       DynamicLinearOperator,
                       type_erasure_table_detail::remove_reference_wrapper_t<
                           std::decay_t< T > > >::add_const_DynamicLinearOperator_ref,
                   &DynamicLinearOperatorDetail::execution_wrapper<
                       DynamicLinearOperator,
                       type_erasure_table_detail::remove_reference_wrapper_t<
                           std::decay_t< T > > >::subtract_const_DynamicLinearOperator_ref,
                   &DynamicLinearOperatorDetail::execution_wrapper<
                       DynamicLinearOperator, type_erasure_table_detail::remove_reference_wrapper_t<
                                                  std::decay_t< T > > >::multiply_double,
                   &DynamicLinearOperatorDetail::execution_wrapper<
                       DynamicLinearOperator, type_erasure_table_detail::remove_reference_wrapper_t<
                                                  std::decay_t< T > > >::negate,
                   &DynamicLinearOperatorDetail::execution_wrapper<
                       DynamicLinearOperator,
                       type_erasure_table_detail::remove_reference_wrapper_t<
                           std::decay_t< T > > >::compare_const_DynamicLinearOperator_ref,
                   &DynamicLinearOperatorDetail::execution_wrapper<
                       DynamicLinearOperator, type_erasure_table_detail::remove_reference_wrapper_t<
                                                  std::decay_t< T > > >::solver,
                   &DynamicLinearOperatorDetail::execution_wrapper<
                       DynamicLinearOperator, type_erasure_table_detail::remove_reference_wrapper_t<
                                                  std::decay_t< T > > >::domain,
                   &DynamicLinearOperatorDetail::execution_wrapper<
                       DynamicLinearOperator, type_erasure_table_detail::remove_reference_wrapper_t<
                                                  std::decay_t< T > > >::range,
                   &DynamicLinearOperatorDetail::execution_wrapper<
                       DynamicLinearOperator, type_erasure_table_detail::remove_reference_wrapper_t<
                                                  std::decay_t< T > > >::space} ),
              impl_( std::forward< T >( value ) )
        {
        }

        template <
            class T,
            typename std::enable_if< DynamicLinearOperatorDetail::Concept<
                DynamicLinearOperator, typename std::decay< T >::type >::value >::type* = nullptr >
        DynamicLinearOperator& operator=( T&& value )
        {
            return *this = DynamicLinearOperator( std::forward< T >( value ) );
        }

        explicit operator bool() const noexcept
        {
            return bool( impl_ );
        }

        /// Apply operator.
        Vector operator()( double t, const Vector& x ) const
        {
            assert( impl_ );
            return function_.call_double_const_Vector_ref( impl_, std::move( t ), x );
        }

        DynamicLinearOperator& operator+=( const DynamicLinearOperator& y )
        {
            assert( impl_ );
            return function_.add_const_DynamicLinearOperator_ref( *this, impl_, y.impl_ );
        }

        DynamicLinearOperator& operator-=( const DynamicLinearOperator& y )
        {
            assert( impl_ );
            return function_.subtract_const_DynamicLinearOperator_ref( *this, impl_, y.impl_ );
        }

        DynamicLinearOperator& operator*=( double a )
        {
            assert( impl_ );
            return function_.multiply_double( *this, impl_, std::move( a ) );
        }

        DynamicLinearOperator operator-() const
        {
            assert( impl_ );
            return function_.negate( impl_ );
        }

        bool operator==( const DynamicLinearOperator& y ) const
        {
            assert( impl_ );
            return function_.compare_const_DynamicLinearOperator_ref( impl_, y.impl_ );
        }

        std::function< Vector( const Vector& ) > solver() const
        {
            assert( impl_ );
            return function_.solver( impl_ );
        }

        /// Access domain space \f$X\f$.
        const VectorSpace& domain() const
        {
            assert( impl_ );
            return function_.domain( impl_ );
        }

        /// Access range space \f$Y\f$.
        const VectorSpace& range() const
        {
            assert( impl_ );
            return function_.range( impl_ );
        }

        /// Access underlying space of linear operators.
        const VectorSpace& space() const
        {
            assert( impl_ );
            return function_.space( impl_ );
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
        DynamicLinearOperatorDetail::Table< DynamicLinearOperator > function_;
        clang::type_erasure::SBOStorage< 16 > impl_;
    };
    /// Type-erased time-dependent differentiable operator \f$A:\ [0,T] \times X \to Y \f$.
    class DynamicC1Operator
    {
    public:
        DynamicC1Operator() noexcept = default;

        template <
            class T,
            typename std::enable_if< DynamicC1OperatorDetail::Concept<
                DynamicC1Operator, typename std::decay< T >::type >::value >::type* = nullptr >
        DynamicC1Operator( T&& value )
            : function_(
                  {&DynamicC1OperatorDetail::execution_wrapper<
                       DynamicC1Operator, type_erasure_table_detail::remove_reference_wrapper_t<
                                              std::decay_t< T > > >::call_double_const_Vector_ref,
                   &DynamicC1OperatorDetail::execution_wrapper<
                       DynamicC1Operator,
                       type_erasure_table_detail::remove_reference_wrapper_t<
                           std::decay_t< T > > >::d1_double_const_Vector_ref_const_Vector_ref,
                   &DynamicC1OperatorDetail::execution_wrapper<
                       DynamicC1Operator,
                       type_erasure_table_detail::remove_reference_wrapper_t<
                           std::decay_t< T > > >::linearization_double_const_Vector_ref,
                   &DynamicC1OperatorDetail::execution_wrapper<
                       DynamicC1Operator, type_erasure_table_detail::remove_reference_wrapper_t<
                                              std::decay_t< T > > >::M,
                   &DynamicC1OperatorDetail::execution_wrapper<
                       DynamicC1Operator, type_erasure_table_detail::remove_reference_wrapper_t<
                                              std::decay_t< T > > >::domain,
                   &DynamicC1OperatorDetail::execution_wrapper<
                       DynamicC1Operator, type_erasure_table_detail::remove_reference_wrapper_t<
                                              std::decay_t< T > > >::range} ),
              impl_( std::forward< T >( value ) )
        {
        }

        template <
            class T,
            typename std::enable_if< DynamicC1OperatorDetail::Concept<
                DynamicC1Operator, typename std::decay< T >::type >::value >::type* = nullptr >
        DynamicC1Operator& operator=( T&& value )
        {
            return *this = DynamicC1Operator( std::forward< T >( value ) );
        }

        explicit operator bool() const noexcept
        {
            return bool( impl_ );
        }

        /// Apply operator.
        Vector operator()( double t, const Vector& x ) const
        {
            assert( impl_ );
            return function_.call_double_const_Vector_ref( impl_, std::move( t ), x );
        }

        /// Compute directional derivative \f$A'(x)\delta x\f$.
        Vector d1( double t, const Vector& x, const Vector& dx ) const
        {
            assert( impl_ );
            return function_.d1_double_const_Vector_ref_const_Vector_ref( impl_, std::move( t ), x,
                                                                          dx );
        }

        /// Get linearization \f$A
        LinearOperator linearization( double t, const Vector& x ) const
        {
            assert( impl_ );
            return function_.linearization_double_const_Vector_ref( impl_, std::move( t ), x );
        }

        LinearOperator M() const
        {
            assert( impl_ );
            return function_.M( impl_ );
        }

        /// Access domain space \f$X\f$.
        const VectorSpace& domain() const
        {
            assert( impl_ );
            return function_.domain( impl_ );
        }

        /// Access range space \f$Y\f$.
        const VectorSpace& range() const
        {
            assert( impl_ );
            return function_.range( impl_ );
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
        DynamicC1OperatorDetail::Table< DynamicC1Operator > function_;
        clang::type_erasure::SBOStorage< 16 > impl_;
    };
}
