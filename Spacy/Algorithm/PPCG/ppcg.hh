/**********************************************************
 *  Author: Jan Zetzmann
 *
 *
 *********************************************************/

#pragma once

#include <iostream>

#include <Spacy/Spaces/ProductSpace/vector.hh>
#include <Spacy/Util/cast.hh>
#include <Spacy/zeroVectorCreator.hh>

#include "Spacy/Algorithm/CG/cg.hh"
#include "Spacy/operator.hh"

#include <Spacy/zeroVectorCreator.hh>

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
        class Solver
        {
            enum class DefiniteNess
            {
                PositiveDefinite,
                Indefinite
            };

        public:
            Solver( Operator A, CallableOperator P, MyOperator proj, MyOperator update_r,
                    MyOperator update_lagr )
                : A_( A ), P_( P ), projection_operator( std::move( proj ) ),
                  update_lagrange( std::move( update_lagr ) ),
                  icg( CG::Solver( std::move( A ), std::move( P ), std::move( update_r ), "ICG" ) )
            {
                icg.setRelativeAccuracy( 1e-6 );
                icg.set_eps( 1e-6 );
                icg.setVerbosity( false );
            }

            /**
            * @brief After solving an equation reports if the underlying operator is indefinite.
            * @return true: if \f$A\f$ is indefinite, else false
            */
            bool indefiniteOperator() const noexcept;

            Vector solve( Vector x, Vector c ) const;

            const CallableOperator& A() const;

            const CallableOperator& P() const;

            ::Spacy::CG::Solver& access_icg() const;

        private:
            Real get_dampening_on_kernel( const Vector& x, const Vector& b,
                                          const Vector& dx ) const;

            /// PPCG Implementation.
            CallableOperator A_, P_;
            MyOperator projection_operator, update_lagrange;
            mutable CG::Solver icg;

            mutable DefiniteNess definiteness_ = DefiniteNess::PositiveDefinite;
        };
    }
}
