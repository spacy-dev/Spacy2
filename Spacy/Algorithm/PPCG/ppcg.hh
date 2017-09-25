/**********************************************************
 *  Author: Jan Zetzmann
 *
 *
 *  TODO: regularization (?), dampening
 *********************************************************/

#ifndef SPACY_KASKADE_PPCG_HH
#define SPACY_KASKADE_PPCG_HH

#pragma once

#include <iostream>

#include <Spacy/Spaces/ProductSpace/vector.hh>
#include <Spacy/Util/cast.hh>
#include <Spacy/zeroVectorCreator.hh>

#include "Spacy/operator.hh"
#include "Spacy/Algorithm/CG/cg.hh"


#include "Spacy/Util/Mixins/accuracy.hh"
#include "Spacy/Util/Mixins/Eps.hh"
#include "Spacy/Util/Mixins/iterativeRefinements.hh"
#include "Spacy/Util/Mixins/maxSteps.hh"
#include "Spacy/Util/Mixins/verbosity.hh"
#include <Spacy/zeroVectorCreator.hh>

#include <Spacy/Algorithm/CG/terminationCriterion.hh>
#include <Spacy/Algorithm/CG/terminationCriteria.hh>


namespace Spacy
{
    /// @cond
    class Vector;
    /// @endcond

    namespace PPCG
    {
        /**
         * @ingroup CGGroup
         * @brief Projected Preconditioned Conjugate gradient method.
         *
         */
        class Solver :
            public Mixin::AbsoluteAccuracy ,
            public Mixin::RelativeAccuracy ,
            public Mixin::Eps ,
            public Mixin::IterativeRefinements ,
            public Mixin::Verbosity,
            public Mixin::MaxSteps
        {
            enum class Result { Converged, Failed, EncounteredNonConvexity, TruncatedAtNonConvexity  };
            enum class DefiniteNess { PositiveDefinite , Indefinite };
        public:
            Solver( Operator A, CallableOperator P, 
                    MyOperator proj, MyOperator update_r, MyOperator update_lagr)
            :   A_( A ), 
                P_( P ), 
                projection_operator( std::move(proj) ), 
                update_lagrange( std::move(update_lagr)),
                icg( CG::Solver(std::move(A), std::move(P), std::move(update_r), "ICG") ),
                terminate( CG::Termination::StrakosTichyEnergyError{} )
                 {}

            /**
            * @brief After solving an equation reports if the underlying operator is indefinite.
            * @return true: if \f$A\f$ is indefinite, else false
            */
            bool indefiniteOperator() const noexcept;
      
            Vector solve( Vector x, const Vector& c ) const;

            const CallableOperator& A() const;

            const CallableOperator& P() const;

        private:

            Real get_dampening_on_kernel(const Vector& x, const Vector& b, const Vector& dx) const;

            // StrakosTichy
            /// Check if step length is below maximal attainable accuracy.
            bool vanishingStep(unsigned step) const;
      
            /**
             * @brief Behaviour if a direction of negative curvature is encountered.
             *
             * Throws if a direction of negative curvature is encountered in the standard cg implementation.
             *
             * @return true if iteration should terminate.
             */
            bool terminateOnNonconvexity(Real qAq, Real qPq, Vector& x, const Vector& q, unsigned step) const;
      

            /// PPCG Implementation.
            CallableOperator    A_, P_;
            MyOperator          projection_operator, update_lagrange;
            mutable CG::Solver  icg;

            // modifications on exact kernel
            bool    is_update_lagrange_multiplier = true,
                    is_calc_dampening = true;

            // StrakosTichy
            mutable CG::TerminationCriterion terminate;
            mutable Result result = Result::Failed; ///< information about reason for termination
            mutable DefiniteNess definiteness_ = DefiniteNess::PositiveDefinite;
        };

    }
}

#endif // SPACY_KASKADE_PPCG_HH