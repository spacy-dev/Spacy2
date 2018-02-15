// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "cg.hh"

#include "terminationCriteria.hh"
#include "Spacy/Util/Exceptions/singularOperatorException.hh"
#include "Spacy/vector.hh"
#include "Spacy/Util/cast.hh"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <utility>

#include "RegularizeViaPreconditioner.hh"
namespace Spacy
{
    namespace CG
    {
        Solver::Solver( CallableOperator A, CallableOperator P, Regularization regularization,
                        bool truncated )
            : A_( std::move( A ) ), P_( std::move( P ) ),
              terminate_( CG::Termination::StrakosTichyEnergyError{} ), truncated_( truncated ),
              regularization_( std::move( regularization ) )
        {
          if(!::Spacy::is<NoRegularization> (regularization_))
            regularized_ = true;
        }

        Vector Solver::solve( const Vector& x, const Vector& b ) const
        {
            regularization_.init();
            definiteness_ = DefiniteNess::PositiveDefinite;
            result = Result::Failed;

            terminate_.set_eps( get( eps() ) );
            terminate_.setRelativeAccuracy( get( getRelativeAccuracy() ) );
            terminate_.setAbsoluteAccuracy( get( getAbsoluteAccuracy() ) );

            if ( !regularization_ )
                return cgLoop( x, b );
            else
            {
                Vector y = x;
                while ( result != Result::Converged && result != Result::TruncatedAtNonConvexity )
                    y = cgLoop( x, b );
                return y;
            }
        }

        CG::TerminationCriterion& Solver::terminationCriterion() noexcept
        {
            return terminate_;
        }

        bool Solver::indefiniteOperator() const noexcept
        {
            return definiteness_ == DefiniteNess::Indefinite;
        }

        const CallableOperator& Solver::P() const
        {
            return P_;
        }

        const CallableOperator& Solver::A() const
        {
            return A_;
        }

        Vector Solver::cgLoop( Vector x, Vector r ) const
        {
            terminate_.clear();
            result = Result::Failed;


            // initialization phase for conjugate gradients
            auto Ax = A_( x );
            r -= Ax;
            auto Qr = Q( r );

            auto q = Qr;
            auto Pq = r; // required only for regularized or hybrid conjugate gradient methods

            auto sigma = abs( r( Qr ) ); // preconditioned residual norm squared

            // the conjugate gradient iteration
            for ( unsigned step = 1; true; step++ )
            {
                // if( getVerbosityLevel() > 1 ) std::cout << "Iteration: " << step << std::endl;
                auto Aq = A_( q );
                Real qAq = Aq( q );
                Real qPq = Pq( q );
                regularization_.apply( qAq, qPq );

                auto alpha = sigma / qAq;
                // if( getVerbosityLevel() > 1 ) std::cout << "    " << "  sigma = " << sigma << ",
                // alpha = " << alpha << ", qAq = " << qAq << ", qPq = " << qPq << std::endl;

                terminate_.update( get( alpha ), get( qAq ), get( qPq ), get( sigma ) );

                //  don't trust small numbers
                if ( vanishingStep( step ) )
                {
                    if ( step == 1 )
                        x += q;
                    result = Result::Converged;
                    break;
                }

                auto Ax = A_( x );
//                std::cout << "qAq/xAx: " << get( qAq ) << "/" << get( Ax( x ) ) << std::endl;
                if ( terminateOnNonconvexity( qAq, qPq, x, q, step ) )
                    break;

                x += ( get( alpha ) * q );

                // convergence test
                if ( terminate_() )
                {
                    if ( verbose() )
                        std::cout << "    "
                                  << ": Terminating in iteration " << step << ".\n";
                    result = ( step == getMaxSteps() ) ? Result::Failed : Result::Converged;
                    break;
                }

                r -= alpha * Aq;
                regularization_.adjustResidual( alpha, Pq, r );

                Qr = Q( r );

                // determine new search direction
                auto sigmaNew = abs( r( Qr ) ); // sigma = <Qr,r>
                auto beta = sigmaNew / sigma;
                sigma = sigmaNew;

                q *= get( beta );
                q += Qr; //  q = Qr + beta*q
                Pq *= get( beta );
                Pq += r; // Pq = r + beta*Pq
            }

            return x;
        }

        Vector Solver::Q( const Vector& r ) const
        {
            auto Qr = P_( r );
            for ( auto i = 0u; i < getIterativeRefinements(); ++i )
                Qr += P_( r - A_( Qr ) );
            return Qr;
        }

        bool Solver::vanishingStep( unsigned step ) const
        {
            if ( terminate_.vanishingStep() )
            {
                if ( verbose() )
                    std::cout
                        << "    "
                        << ": Terminating due to numerically almost vanishing step in iteration "
                        << step << "." << std::endl;
                result = Result::Converged;
                return true;
            }
            return false;
        }

        bool Solver::terminateOnNonconvexity( Real qAq, Real qPq, Vector& x, const Vector& q,
                                              unsigned step ) const
        {
            if ( qAq > 0 )
                return false;
            if ( verbose() )
                std::cout << "    "
                          << ": Negative curvature: " << qAq << std::endl;

            if ( !truncated_ && !regularized_ )
            {
                if ( verbose() )
                {
                    std::cout << "    "
                              << ": Direction of negative curvature encountered in standard CG "
                                 "Implementation!"
                              << std::endl;
                    std::cout << "    "
                              << ": Either something is wrong with your operator or you should use "
                                 "TCG, RCG or TRCG. Terminating CG!"
                              << std::endl;
                }

                throw SingularOperatorException( "CG::terminateOnNonconvexity" );
            }

            if ( truncated_ && ( !regularized_ || terminate_.minimalDecreaseAchieved() ) )
            {
                // At least do something to retain a little chance to get out of the nonconvexity.
                // If a nonconvexity is encountered in the first step something probably went wrong
                // elsewhere. Chances that a way out of the nonconvexity can be found are small in
                // this case.
                if ( step == 1 )
                    x += q;
                if ( verbose() )
                    std::cout << "    "
                              << ": Truncating at nonconvexity in iteration " << step << ": " << qAq
                              << std::endl;
                definiteness_ = DefiniteNess::Indefinite;
                result = Result::TruncatedAtNonConvexity;
                return true;
            }

            regularization_.update( qAq, qPq );
            if ( verbose() )
                std::cout << "    "
                          << ": Regularizing at nonconvexity in iteration " << step << "."
                          << std::endl;
            definiteness_ = DefiniteNess::Indefinite;
            result = Result::EncounteredNonConvexity;
            return true;
        }
    }
}
