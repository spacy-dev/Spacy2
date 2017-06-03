// This file was automatically generated using clang-type-erase.
// Please do not modify.

#pragma once

#include <Spacy/Detail/zeroVectorCreator_table.h>
#include <Spacy/Util/storage.h>

#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>
namespace Spacy
{
    /// Each VectorSpace needs a zero-vector creator to support generation of vector space elements.
    class ZeroVectorCreator
    {
    public:
        ZeroVectorCreator() noexcept = default;

        template <
            class T,
            typename std::enable_if< ZeroVectorCreatorDetail::Concept<
                ZeroVectorCreator, typename std::decay< T >::type >::value >::type* = nullptr >
        ZeroVectorCreator( T&& value )
            : function_( {&ZeroVectorCreatorDetail::execution_wrapper<
                  ZeroVectorCreator, type_erasure_table_detail::remove_reference_wrapper_t<
                                         std::decay_t< T > > >::call_const_VectorSpace_ptr} ),
              impl_( std::forward< T >( value ) )
        {
        }

        template <
            class T,
            typename std::enable_if< ZeroVectorCreatorDetail::Concept<
                ZeroVectorCreator, typename std::decay< T >::type >::value >::type* = nullptr >
        ZeroVectorCreator& operator=( T&& value )
        {
            return *this = ZeroVectorCreator( std::forward< T >( value ) );
        }

        explicit operator bool() const noexcept
        {
            return bool( impl_ );
        }

        /// Creates \f$
        Vector operator()( const VectorSpace* V ) const
        {
            assert( impl_ );
            return function_.call_const_VectorSpace_ptr( impl_, V );
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
        ZeroVectorCreatorDetail::Table< ZeroVectorCreator > function_;
        clang::type_erasure::Storage impl_;
    };
    template < class T >
    T& creator( VectorSpace& space )
    {
        return *space.creator().template target< T >();
    }

    template < class T >
    const T& creator( const VectorSpace& space )
    {
        return *space.creator().template target< T >();
    }

    /// Create new vector \f$v=0\f$.
    inline Vector zero( const VectorSpace& space )
    {
        return space.creator()( &space );
    }
}
