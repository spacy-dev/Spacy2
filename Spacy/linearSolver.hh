// This file was automatically generated using clang-type-erase.
// Please do not modify.

#pragma once

#include <Spacy/Detail/linearSolver_table.h>
#include <Spacy/Util/storage.h>

#include <functional>
#include <Spacy/vector.hh>
namespace Spacy
{
    /// Type-erased linear solver.
    using LinearSolver = std::function< Vector( const Vector& ) >;
    /// Type-erased indefinite linear solver. Additionally monitors if the underlying operator is
    /// positive definite.
    class IndefiniteLinearSolver
    {
    public:
        IndefiniteLinearSolver() noexcept = default;

        template <
            class T,
            typename std::enable_if< IndefiniteLinearSolverDetail::Concept<
                IndefiniteLinearSolver, typename std::decay< T >::type >::value >::type* = nullptr >
        IndefiniteLinearSolver( T&& value )
            : function_( {&IndefiniteLinearSolverDetail::execution_wrapper<
                              IndefiniteLinearSolver,
                              type_erasure_table_detail::remove_reference_wrapper_t<
                                  std::decay_t< T > > >::call_const_Vector_ref,
                          &IndefiniteLinearSolverDetail::execution_wrapper<
                              IndefiniteLinearSolver,
                              type_erasure_table_detail::remove_reference_wrapper_t<
                                  std::decay_t< T > > >::isPositiveDefinite} ),
              impl_( std::forward< T >( value ) )
        {
        }

        template <
            class T,
            typename std::enable_if< IndefiniteLinearSolverDetail::Concept<
                IndefiniteLinearSolver, typename std::decay< T >::type >::value >::type* = nullptr >
        IndefiniteLinearSolver& operator=( T&& value )
        {
            return *this = IndefiniteLinearSolver( std::forward< T >( value ) );
        }

        explicit operator bool() const noexcept
        {
            return bool( impl_ );
        }

        Vector operator()( const Vector& x ) const
        {
            assert( impl_ );
            return function_.call_const_Vector_ref( impl_, x );
        }

        bool isPositiveDefinite() const
        {
            assert( impl_ );
            return function_.isPositiveDefinite( impl_ );
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
        IndefiniteLinearSolverDetail::Table< IndefiniteLinearSolver > function_;
        clang::type_erasure::Storage impl_;
    };
}
