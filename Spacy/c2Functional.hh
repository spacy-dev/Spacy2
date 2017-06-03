// This file was automatically generated using clang-type-erase.
// Please do not modify.

#pragma once

#include <Spacy/Detail/c2Functional_table.h>
#include <Spacy/Util/storage.h>

#include <Spacy/Spaces/ScalarSpace/Real.hh>
#include <Spacy/linearOperator.hh>
#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>
namespace Spacy
{
    /// Type-erased twice differentiable functional \f$f:\ X \to \mathbb{R} \f$.
    class C2Functional
    {
    public:
        C2Functional() noexcept = default;

        template < class T,
                   typename std::enable_if< C2FunctionalDetail::Concept<
                       C2Functional, typename std::decay< T >::type >::value >::type* = nullptr >
        C2Functional( T&& value )
            : function_( {&C2FunctionalDetail::execution_wrapper<
                              C2Functional, type_erasure_table_detail::remove_reference_wrapper_t<
                                                std::decay_t< T > > >::call_const_Vector_ref,
                          &C2FunctionalDetail::execution_wrapper<
                              C2Functional, type_erasure_table_detail::remove_reference_wrapper_t<
                                                std::decay_t< T > > >::d1_const_Vector_ref,
                          &C2FunctionalDetail::execution_wrapper<
                              C2Functional,
                              type_erasure_table_detail::remove_reference_wrapper_t<
                                  std::decay_t< T > > >::d2_const_Vector_ref_const_Vector_ref,
                          &C2FunctionalDetail::execution_wrapper<
                              C2Functional, type_erasure_table_detail::remove_reference_wrapper_t<
                                                std::decay_t< T > > >::hessian_const_Vector_ref,
                          &C2FunctionalDetail::execution_wrapper<
                              C2Functional, type_erasure_table_detail::remove_reference_wrapper_t<
                                                std::decay_t< T > > >::domain} ),
              impl_( std::forward< T >( value ) )
        {
        }

        template < class T,
                   typename std::enable_if< C2FunctionalDetail::Concept<
                       C2Functional, typename std::decay< T >::type >::value >::type* = nullptr >
        C2Functional& operator=( T&& value )
        {
            return *this = C2Functional( std::forward< T >( value ) );
        }

        explicit operator bool() const noexcept
        {
            return bool( impl_ );
        }

        /// Apply functional.
        Real operator()( const Vector& x ) const
        {
            assert( impl_ );
            return function_.call_const_Vector_ref( impl_, x );
        }

        /// Compute derivative as function space element in \f$X^*\f$, where \f$x\in X\f$.
        Vector d1( const Vector& x ) const
        {
            assert( impl_ );
            return function_.d1_const_Vector_ref( impl_, x );
        }

        /// Compute second derivative as function space element in \f$X^*\f$, where \f$x,dx\in X\f$.
        Vector d2( const Vector& x, const Vector& dx ) const
        {
            assert( impl_ );
            return function_.d2_const_Vector_ref_const_Vector_ref( impl_, x, dx );
        }

        /// Access hessian as linear operator \f$ X \rightarrow X^\f$.
        LinearOperator hessian( const Vector& x ) const
        {
            assert( impl_ );
            return function_.hessian_const_Vector_ref( impl_, x );
        }

        /// Access domain space \f$X\f$.
        const VectorSpace& domain() const
        {
            assert( impl_ );
            return function_.domain( impl_ );
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
        C2FunctionalDetail::Table< C2Functional > function_;
        clang::type_erasure::Storage impl_;
    };
    /// @brief For a functional \f$ f: X\to \mathbb{R} \f$, compute \f$f'\f$ at \f$x\in X\f$ as dual
    /// element \f$ f'(x) \in X^\f$.
    ///   *
    /// Equivalent to calling f.d1(x).
    ///   *
    ///@param f twice differentiable functional
    ///@param x point of linearization
    ///@return \f$f'(x)\f$, i.e. f.d1(x).
    ///@see C2Functional
    inline Vector d1( const C2Functional& f, const Vector& x )
    {
        return f.d1( x );
    }

    /// @brief For a functional \f$ f: X\to \mathbb{R} \f$, compute \f$f''\f$ at \f$x\in X\f$ as
    /// linear operator \f$ f''(x): X \to X^\f$.
    ///   *
    /// Equivalent to calling f.hessian(x).
    ///   *
    ///@param f twice differentiable functional
    ///@param x point of linearization
    ///@return \f$f''(x)\f$, i.e. f.hessian(x).
    ///@see C2Functional
    inline LinearOperator d2( const C2Functional& f, const Vector& x )
    {
        return f.hessian( x );
    }
}
