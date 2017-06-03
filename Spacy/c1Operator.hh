// This file was automatically generated using clang-type-erase.
// Please do not modify.

#pragma once

#include <Spacy/Detail/c1Operator_table.h>
#include <Spacy/Util/storage.h>

#include <Spacy/linearOperator.hh>
#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>
namespace Spacy
{
    /// Type-erased differentiable operator \f$A:\ X \to Y \f$.
    class C1Operator
    {
    public:
        C1Operator() noexcept = default;

        template < class T,
                   typename std::enable_if< C1OperatorDetail::Concept<
                       C1Operator, typename std::decay< T >::type >::value >::type* = nullptr >
        C1Operator( T&& value )
            : function_(
                  {&C1OperatorDetail::execution_wrapper<
                       C1Operator, type_erasure_table_detail::remove_reference_wrapper_t<
                                       std::decay_t< T > > >::call_const_Vector_ref,
                   &C1OperatorDetail::execution_wrapper<
                       C1Operator, type_erasure_table_detail::remove_reference_wrapper_t<
                                       std::decay_t< T > > >::d1_const_Vector_ref_const_Vector_ref,
                   &C1OperatorDetail::execution_wrapper<
                       C1Operator, type_erasure_table_detail::remove_reference_wrapper_t<
                                       std::decay_t< T > > >::linearization_const_Vector_ref,
                   &C1OperatorDetail::execution_wrapper<
                       C1Operator, type_erasure_table_detail::remove_reference_wrapper_t<
                                       std::decay_t< T > > >::domain,
                   &C1OperatorDetail::execution_wrapper<
                       C1Operator, type_erasure_table_detail::remove_reference_wrapper_t<
                                       std::decay_t< T > > >::range} ),
              impl_( std::forward< T >( value ) )
        {
        }

        template < class T,
                   typename std::enable_if< C1OperatorDetail::Concept<
                       C1Operator, typename std::decay< T >::type >::value >::type* = nullptr >
        C1Operator& operator=( T&& value )
        {
            return *this = C1Operator( std::forward< T >( value ) );
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

        /// Compute directional derivative \f$A'(x)\delta x\f$.
        Vector d1( const Vector& x, const Vector& dx ) const
        {
            assert( impl_ );
            return function_.d1_const_Vector_ref_const_Vector_ref( impl_, x, dx );
        }

        /// Get linearization \f$A
        LinearOperator linearization( const Vector& x ) const
        {
            assert( impl_ );
            return function_.linearization_const_Vector_ref( impl_, x );
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
        C1OperatorDetail::Table< C1Operator > function_;
        clang::type_erasure::Storage impl_;
    };
    /// @brief For an operator \f$ A: X\to Y \f$, compute \f$A'\f$ at \f$x\in X\f$ as linear
    /// operator \f$ A'(x): X \to Y \f$.
    ///   *
    /// Equivalent to calling A.linearization(x).
    ///   *
    ///@param A differentiable operator
    ///@param x point of linearization
    ///@return \f$A'(x)\f$, i.e. A.linearization(x).
    ///@see @ref C1OperatorAnchor "C1Operator", @ref LinearOperatorAnchor "LinearOperator"
    ///
    inline LinearOperator d1( const C1Operator& A, const Vector& x )
    {
        return A.linearization( x );
    }
}
