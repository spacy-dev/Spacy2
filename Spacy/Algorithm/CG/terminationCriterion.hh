// This file was automatically generated using clang-type-erase.
// Please do not modify.

#pragma once

#include <Spacy/Algorithm/CG/Detail/terminationCriterion_table.h>
#include <Spacy/Util/storage.h>

namespace Spacy
{
    namespace CG
    {
        /// Type-erased termination criterion for conjugate gradient methods.
        class TerminationCriterion
        {
        public:
            TerminationCriterion() noexcept = default;

            template < class T,
                       typename std::enable_if< TerminationCriterionDetail::Concept<
                           TerminationCriterion, typename std::decay< T >::type >::value >::type* =
                           nullptr >
            TerminationCriterion( T&& value )
                : function_( {&TerminationCriterionDetail::execution_wrapper<
                                  TerminationCriterion,
                                  type_erasure_table_detail::remove_reference_wrapper_t<
                                      std::decay_t< T > > >::call,
                              &TerminationCriterionDetail::execution_wrapper<
                                  TerminationCriterion,
                                  type_erasure_table_detail::remove_reference_wrapper_t<
                                      std::decay_t< T > > >::clear,
                              &TerminationCriterionDetail::execution_wrapper<
                                  TerminationCriterion,
                                  type_erasure_table_detail::remove_reference_wrapper_t<
                                      std::decay_t< T > > >::update_double_double_double_double,
                              &TerminationCriterionDetail::execution_wrapper<
                                  TerminationCriterion,
                                  type_erasure_table_detail::remove_reference_wrapper_t<
                                      std::decay_t< T > > >::vanishingStep,
                              &TerminationCriterionDetail::execution_wrapper<
                                  TerminationCriterion,
                                  type_erasure_table_detail::remove_reference_wrapper_t<
                                      std::decay_t< T > > >::minimalDecreaseAchieved,
                              &TerminationCriterionDetail::execution_wrapper<
                                  TerminationCriterion,
                                  type_erasure_table_detail::remove_reference_wrapper_t<
                                      std::decay_t< T > > >::set_eps_double,
                              &TerminationCriterionDetail::execution_wrapper<
                                  TerminationCriterion,
                                  type_erasure_table_detail::remove_reference_wrapper_t<
                                      std::decay_t< T > > >::setAbsoluteAccuracy_double,
                              &TerminationCriterionDetail::execution_wrapper<
                                  TerminationCriterion,
                                  type_erasure_table_detail::remove_reference_wrapper_t<
                                      std::decay_t< T > > >::setMinimalAccuracy_double,
                              &TerminationCriterionDetail::execution_wrapper<
                                  TerminationCriterion,
                                  type_erasure_table_detail::remove_reference_wrapper_t<
                                      std::decay_t< T > > >::setRelativeAccuracy_double} ),
                  impl_( std::forward< T >( value ) )
            {
            }

            template < class T,
                       typename std::enable_if< TerminationCriterionDetail::Concept<
                           TerminationCriterion, typename std::decay< T >::type >::value >::type* =
                           nullptr >
            TerminationCriterion& operator=( T&& value )
            {
                return *this = TerminationCriterion( std::forward< T >( value ) );
            }

            explicit operator bool() const noexcept
            {
                return bool( impl_ );
            }

            bool operator()() const
            {
                assert( impl_ );
                return function_.call( impl_ );
            }

            void clear()
            {
                assert( impl_ );
                function_.clear( impl_ );
            }

            void update( double alpha, double qAq, double qPq, double rPINVr )
            {
                assert( impl_ );
                function_.update_double_double_double_double( impl_, std::move( alpha ),
                                                              std::move( qAq ), std::move( qPq ),
                                                              std::move( rPINVr ) );
            }

            bool vanishingStep() const
            {
                assert( impl_ );
                return function_.vanishingStep( impl_ );
            }

            bool minimalDecreaseAchieved() const
            {
                assert( impl_ );
                return function_.minimalDecreaseAchieved( impl_ );
            }

            void set_eps( double eps )
            {
                assert( impl_ );
                function_.set_eps_double( impl_, std::move( eps ) );
            }

            void setAbsoluteAccuracy( double accuracy )
            {
                assert( impl_ );
                function_.setAbsoluteAccuracy_double( impl_, std::move( accuracy ) );
            }

            void setMinimalAccuracy( double accuracy )
            {
                assert( impl_ );
                function_.setMinimalAccuracy_double( impl_, std::move( accuracy ) );
            }

            void setRelativeAccuracy( double accuracy )
            {
                assert( impl_ );
                function_.setRelativeAccuracy_double( impl_, std::move( accuracy ) );
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
            TerminationCriterionDetail::Table< TerminationCriterion > function_;
            clang::type_erasure::Storage impl_;
        };
    }
}
