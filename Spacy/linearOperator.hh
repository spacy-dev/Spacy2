// This file was automatically generated using clang-type-erase.
// Please do not modify.

#pragma once

#include <Spacy/Detail/linearOperator_table.h>
#include <Spacy/Util/storage.h>

#include <functional>
#include <Spacy/Spaces/ScalarSpace/Real.hh>
#include <Spacy/Util/Exceptions/invalidArgumentException.hh>
#include <Spacy/linearSolver.hh>
#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>
namespace Spacy
{
    /// Type-erased linear operator \f$A:\ X \to Y \f$.
    class LinearOperator
    {
    public:
        LinearOperator() noexcept = default;

        template < class T,
                   typename std::enable_if< LinearOperatorDetail::Concept<
                       LinearOperator, typename std::decay< T >::type >::value >::type* = nullptr >
        LinearOperator( T&& value )
            : function_(
                  {&LinearOperatorDetail::execution_wrapper<
                       LinearOperator, type_erasure_table_detail::remove_reference_wrapper_t<
                                           std::decay_t< T > > >::call_const_Vector_ref,
                   &LinearOperatorDetail::execution_wrapper<
                       LinearOperator, type_erasure_table_detail::remove_reference_wrapper_t<
                                           std::decay_t< T > > >::call_const_LinearOperator_ref,
                   &LinearOperatorDetail::execution_wrapper<
                       LinearOperator, type_erasure_table_detail::remove_reference_wrapper_t<
                                           std::decay_t< T > > >::add_const_LinearOperator_ref,
                   &LinearOperatorDetail::execution_wrapper<
                       LinearOperator, type_erasure_table_detail::remove_reference_wrapper_t<
                                           std::decay_t< T > > >::subtract_const_LinearOperator_ref,
                   &LinearOperatorDetail::execution_wrapper<
                       LinearOperator, type_erasure_table_detail::remove_reference_wrapper_t<
                                           std::decay_t< T > > >::multiply_double,
                   &LinearOperatorDetail::execution_wrapper<
                       LinearOperator, type_erasure_table_detail::remove_reference_wrapper_t<
                                           std::decay_t< T > > >::negate,
                   &LinearOperatorDetail::execution_wrapper<
                       LinearOperator, type_erasure_table_detail::remove_reference_wrapper_t<
                                           std::decay_t< T > > >::compare_const_LinearOperator_ref,
                   &LinearOperatorDetail::execution_wrapper<
                       LinearOperator, type_erasure_table_detail::remove_reference_wrapper_t<
                                           std::decay_t< T > > >::solver,
                   &LinearOperatorDetail::execution_wrapper<
                       LinearOperator, type_erasure_table_detail::remove_reference_wrapper_t<
                                           std::decay_t< T > > >::domain,
                   &LinearOperatorDetail::execution_wrapper<
                       LinearOperator, type_erasure_table_detail::remove_reference_wrapper_t<
                                           std::decay_t< T > > >::range,
                   &LinearOperatorDetail::execution_wrapper<
                       LinearOperator, type_erasure_table_detail::remove_reference_wrapper_t<
                                           std::decay_t< T > > >::space} ),
              impl_( std::forward< T >( value ) )
        {
        }

        template < class T,
                   typename std::enable_if< LinearOperatorDetail::Concept<
                       LinearOperator, typename std::decay< T >::type >::value >::type* = nullptr >
        LinearOperator& operator=( T&& value )
        {
            return *this = LinearOperator( std::forward< T >( value ) );
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

        Real operator()( const LinearOperator& x ) const
        {
            assert( impl_ );
            return function_.call_const_LinearOperator_ref( impl_, x.impl_ );
        }

        LinearOperator& operator+=( const LinearOperator& y )
        {
            assert( impl_ );
            return function_.add_const_LinearOperator_ref( *this, impl_, y.impl_ );
        }

        LinearOperator& operator-=( const LinearOperator& y )
        {
            assert( impl_ );
            return function_.subtract_const_LinearOperator_ref( *this, impl_, y.impl_ );
        }

        LinearOperator& operator*=( double a )
        {
            assert( impl_ );
            return function_.multiply_double( *this, impl_, std::move( a ) );
        }

        LinearOperator operator-() const
        {
            assert( impl_ );
            return function_.negate( impl_ );
        }

        bool operator==( const LinearOperator& y ) const
        {
            assert( impl_ );
            return function_.compare_const_LinearOperator_ref( impl_, y.impl_ );
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
        LinearOperatorDetail::Table< LinearOperator > function_;
        clang::type_erasure::SBOStorage< 16 > impl_;
    };
    /// Access solver via A^-1. Throws for k!=-1.
    inline LinearSolver operator^( const LinearOperator& A, int k )
    {
        if ( k == -1 )
            return A.solver();
        throw Exception::InvalidArgument(
            "operator^ for LinearOperator only defined for exponent: k = -1." );
    }

    /// Access solver via A^-1. Throws for k!=-1.
    inline LinearSolver operator^( LinearOperator&& A, int k )
    {
        if ( k == -1 )
            return A.solver();
        throw Exception::InvalidArgument(
            "operator^ for LinearOperator only defined for exponent: k = -1." );
    }

    inline LinearOperator& axpy( LinearOperator& A, double a, LinearOperator B )
    {
        return A += ( B *= a );
    }
}
