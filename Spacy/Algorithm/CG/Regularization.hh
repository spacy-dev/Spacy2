// This file was automatically generated using clang-type-erase.
// Please do not modify.

#pragma once

#include <Spacy/Algorithm/CG/Detail/Regularization_table.h>
#include <Spacy/Util/storage.h>

#include <Spacy/Spaces/ScalarSpace/Real.hh>
#include <Spacy/vector.hh>
namespace Spacy
{
    namespace CG
    {
        /// Regularization for conjugate gradient methods.
        class Regularization
        {
        public:
            Regularization() noexcept = default;

            template <
                class T,
                typename std::enable_if< RegularizationDetail::Concept<
                    Regularization, typename std::decay< T >::type >::value >::type* = nullptr >
            Regularization( T&& value )
                : function_(
                      {&RegularizationDetail::execution_wrapper<
                           Regularization, type_erasure_table_detail::remove_reference_wrapper_t<
                                               std::decay_t< T > > >::init,
                       &RegularizationDetail::execution_wrapper<
                           Regularization, type_erasure_table_detail::remove_reference_wrapper_t<
                                               std::decay_t< T > > >::apply_Real_ref_Real,
                       &RegularizationDetail::execution_wrapper<
                           Regularization, type_erasure_table_detail::remove_reference_wrapper_t<
                                               std::decay_t< T > > >::update_Real_Real,
                       &RegularizationDetail::execution_wrapper<
                           Regularization,
                           type_erasure_table_detail::remove_reference_wrapper_t< std::decay_t<
                               T > > >::adjustResidual_Real_const_Vector_ref_Vector_ref} ),
                  impl_( std::forward< T >( value ) )
            {
            }

            template <
                class T,
                typename std::enable_if< RegularizationDetail::Concept<
                    Regularization, typename std::decay< T >::type >::value >::type* = nullptr >
            Regularization& operator=( T&& value )
            {
                return *this = Regularization( std::forward< T >( value ) );
            }

            explicit operator bool() const noexcept
            {
                return bool( impl_ );
            }

            /// Initialize regularization.
            void init()
            {
                assert( impl_ );
                function_.init( impl_ );
            }

            /// @brief Apply regularization to qAq, where q is the conjugate search direction.
            ///@param qAq \f$ qAq \f$, where \f$q\f$ is the conjugate search direction and \f$A\f$
            /// the system operator
            ///@param qPq \f$ qPq \f$, where \f$q\f$ is the conjugate search direction and \f$P\f$
            /// the preconditioner
            void apply( Real& qAq, Real qPq ) const
            {
                assert( impl_ );
                function_.apply_Real_ref_Real( impl_, qAq, std::move( qPq ) );
            }

            /// @brief Update regularization (parameter).
            ///@param qAq \f$ qAq \f$, where \f$q\f$ is the conjugate search direction and \f$A\f$
            /// the system operator
            ///@param qPq \f$ qPq \f$, where \f$q\f$ is the conjugate search direction and \f$P\f$
            /// the preconditioner
            void update( Real qAq, Real qPq )
            {
                assert( impl_ );
                function_.update_Real_Real( impl_, std::move( qAq ), std::move( qPq ) );
            }

            /// @brief Adjust residual for consistency with the regularized left hand side.
            ///@param alpha step length parameter of the conjugate gradient method
            ///@param Pq \f$ Pq \f$, where \f$q\f$ is the conjugate search direction and \f$P\f$ the
            /// preconditioner
            ///@param r residual
            void adjustResidual( Real alpha, const Vector& Pq, Vector& r ) const
            {
                assert( impl_ );
                function_.adjustResidual_Real_const_Vector_ref_Vector_ref(
                    impl_, std::move( alpha ), Pq, r );
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
            RegularizationDetail::Table< Regularization > function_;
            clang::type_erasure::Storage impl_;
        };
    }
}
