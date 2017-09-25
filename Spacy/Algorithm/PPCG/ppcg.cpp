/**********************************************************
 *  Author: Jan Zetzmann
 *
 *********************************************************/

#include "ppcg.hh"

#include "Spacy/Spaces/ProductSpace/vector.hh"

namespace Spacy
{
    /// @cond
    class Vector;
    /// @endcond

    namespace // anonymous namepsace like in affineCovariantSolver.cpp to keep this stuff acessible
              // only from this file
    {
        using SV = typename ::Spacy::Vector;
        using SPV = typename ::Spacy::ProductSpace::Vector;

        auto primal_projection( const SV& v )
        {
            SV w = v;
            SPV& w_ =
                cast_ref< SPV >( w ); // w_ is SPV such that you can access primal and dual elements
            w_.component( DUAL ) *= 0;
            return w;
        }
    }

    namespace PPCG
    {

        const CallableOperator& Solver::A() const
        {
            return this->PPCG::Solver::A_;
        }

        const CallableOperator& Solver::P() const
        {
            return this->PPCG::Solver::P_;
        }

        Spacy::CG::Solver& Solver::access_icg() const
        {
            return icg;
        }

        bool Solver::indefiniteOperator() const noexcept
        {
            return definiteness_ == DefiniteNess::Indefinite;
        }

        SV Solver::solve( SV x, SV b ) const
        {
            /**
                Solve with inexact cg, project back on kernel, dampen and update lagrange
               multiplicator
            */

            assert( is< SPV >( x ) );
            assert( is< SPV >( b ) );

            b *= -1.0; // for consistency with spacy's

            SV dx( x ), r, xs( x );             // step
            Real qAq = 0, alpha = 0, sigma = 0; // values for StrakosTichy convergence test

            for ( int step = 0; step < 50; ++step )
            {
                // solve on inexact kernel

                // update lagrange update
                x = update_lagrange( x, b );

                xs = icg.solve( x, b );

                // solve projection on actual kernel
                dx = projection_operator( x, xs );

                dx *= get( get_dampening_on_kernel( x, b, dx ) );

                // go step
                x += primal_projection( dx );
            }
            return x;
        }

        Real Solver::get_dampening_on_kernel( const SV& x, const SV& b, const SV& dx ) const
        {
            /*
                w = - \frac{Lx+b)^T dx}{dx^T Ldx}
            */
            if ( get( dx( dx ) ) == 0 )
                return 1.0;

            SV Lx = primal_projection( A_( primal_projection( x ) ) );
            SV Ldx = primal_projection( A_( primal_projection( dx ) ) );
            return -1.0 *
                   ( ( primal_projection( dx )( Lx + b ) ) / ( primal_projection( dx )( Ldx ) ) );
        }
    }
}