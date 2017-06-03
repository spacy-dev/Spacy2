// This file was automatically generated using clang-type-erase.
// Please do not modify.

#pragma once

#include <Spacy/Detail/vector_table.h>
#include <Spacy/Util/storage.h>

#include <type_traits>
#include <functional>
#include <Spacy/Spaces/ScalarSpace/Real.hh>
#include <Spacy/Util/Exceptions/incompatibleSpaceException.hh>
#include <Spacy/Util/Mixins/Get.hh>
#include <Spacy/vectorSpace.hh>
namespace Spacy
{
    /// Type-erased vector.
    class Vector
    {
    public:
        Vector() noexcept = default;

        template < class T, typename std::enable_if< VectorDetail::Concept<
                                Vector, typename std::decay< T >::type >::value >::type* = nullptr >
        Vector( T&& value )
            : function_( {&VectorDetail::execution_wrapper<
                              Vector, type_erasure_table_detail::remove_reference_wrapper_t<
                                          std::decay_t< T > > >::call_const_Vector_ref,
                          &VectorDetail::execution_wrapper<
                              Vector, type_erasure_table_detail::remove_reference_wrapper_t<
                                          std::decay_t< T > > >::add_const_Vector_ref,
                          &VectorDetail::execution_wrapper<
                              Vector, type_erasure_table_detail::remove_reference_wrapper_t<
                                          std::decay_t< T > > >::subtract_const_Vector_ref,
                          &VectorDetail::execution_wrapper<
                              Vector, type_erasure_table_detail::remove_reference_wrapper_t<
                                          std::decay_t< T > > >::multiply_double,
                          &VectorDetail::execution_wrapper<
                              Vector, type_erasure_table_detail::remove_reference_wrapper_t<
                                          std::decay_t< T > > >::negate,
                          &VectorDetail::execution_wrapper<
                              Vector, type_erasure_table_detail::remove_reference_wrapper_t<
                                          std::decay_t< T > > >::compare_const_Vector_ref,
                          &VectorDetail::execution_wrapper<
                              Vector, type_erasure_table_detail::remove_reference_wrapper_t<
                                          std::decay_t< T > > >::space} ),
              impl_( std::forward< T >( value ) )
        {
        }

        template < class T, typename std::enable_if< VectorDetail::Concept<
                                Vector, typename std::decay< T >::type >::value >::type* = nullptr >
        Vector& operator=( T&& value )
        {
            return *this = Vector( std::forward< T >( value ) );
        }

        explicit operator bool() const noexcept
        {
            return bool( impl_ );
        }

        /// Apply as dual space element.
        Real operator()( const Vector& x ) const
        {
            assert( impl_ );
            return function_.call_const_Vector_ref( impl_, x.impl_ );
        }

        Vector& operator+=( const Vector& y )
        {
            assert( impl_ );
            return function_.add_const_Vector_ref( *this, impl_, y.impl_ );
        }

        Vector& operator-=( const Vector& y )
        {
            assert( impl_ );
            return function_.subtract_const_Vector_ref( *this, impl_, y.impl_ );
        }

        Vector& operator*=( double a )
        {
            assert( impl_ );
            return function_.multiply_double( *this, impl_, std::move( a ) );
        }

        Vector operator-() const
        {
            assert( impl_ );
            return function_.negate( impl_ );
        }

        bool operator==( const Vector& y ) const
        {
            assert( impl_ );
            return function_.compare_const_Vector_ref( impl_, y.impl_ );
        }

        /// Access underlying space.
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
        VectorDetail::Table< Vector > function_;
        clang::type_erasure::Storage impl_;
    };
    /// Multiplication with arithmetic types (double,float,int,...).
    template < class Arithmetic,
               class = std::enable_if_t< std::is_arithmetic< Arithmetic >::value > >
    Vector operator*( Arithmetic a, Vector x )
    {
        return x *= a;
    }

    /// Multiplication with arithmetic types (double,float,int,...).
    template < class Arithmetic,
               class = std::enable_if_t< std::is_arithmetic< Arithmetic >::value > >
    Vector operator*( Vector x, Arithmetic a )
    {
        return x *= a;
    }

    /// Sum of vectors \f$z=x+y\f$.
    inline Vector operator+( Vector x, const Vector& y )
    {
        return x += y;
    }

    /// Subtract vectors \f$z=x-y\f$.
    inline Vector operator-( Vector x, const Vector& y )
    {
        return x -= y;
    }

    /// Compute scalar product \f$z=x*y=(x,y)\f$.
    inline Real operator*( const Vector& x, const Vector& y )
    {
        return x.space().scalarProduct()( x, y );
    }

    /// Compute norm, where the norm associated with the underlying function space is used \f$ z =
    /// \|x\| \f$.
    inline Real norm( const Vector& x )
    {
        return x.space().norm()( x );
    }

    inline void checkDualPairing( const Vector& x, const Vector& y )
    {
        if ( !y.space().isPrimalWRT( x.space() ) )
            throw IncompatibleSpaceException( x.space().index(), y.space().index() );
    }

    template < class T, typename std::enable_if< std::is_arithmetic< T >::value >::type* = nullptr >
    Vector operator*( const Mixin::Get< T >& x, Vector y )
    {
        return y *= get( x );
    }

    template < class T, typename std::enable_if< std::is_arithmetic< T >::value >::type* = nullptr >
    Vector operator*( const Vector& x, const Mixin::Get< T >& y )
    {
        return y * x;
    }

    inline Vector operator*( const Mixin::Get< Real >& x, Vector y )
    {
        return y *= get( get( x ) );
    }

    inline Vector operator*( const Vector& x, const Mixin::Get< Real >& y )
    {
        return y * x;
    }
}
