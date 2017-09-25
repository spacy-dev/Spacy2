// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "cg.hh"

#include "terminationCriteria.hh"
#include "Spacy/Util/Exceptions/singularOperatorException.hh"
#include "Spacy/vector.hh"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <utility>

#include <Spacy/Util/cast.hh> // for cast_ref in primalProjection
#include <Spacy/Spaces/ProductSpace/vector.hh> // for ProductSpace::Vector in primalProjection

namespace Spacy
{

    namespace // anonymous namepsace like in affineCovariantSolver.cpp to keep this stuff acessible only from this file
    {
        using SV  = typename ::Spacy::Vector;
        using SPV = typename ::Spacy::ProductSpace::Vector;

        auto primal_projection( const SV& v )
        {
            SV w = v;
            SPV& w_ = cast_ref< SPV >( w ); // w_ is SPV such that you can access primal and dual elements
            w_.component( DUAL ) *= 0;
            return w;
        }

        // "Preconditioner for solving the preconditioner problem"
        auto Identity_Preconditioner = []( const SV& x_loc ) -> SV { return x_loc; };
    }

    namespace CG
    {
        Solver::Solver( CallableOperator A, CallableOperator P, const std::string& type )
            : A_( std::move( A ) ), P_( std::move( P ) ),
            update_r_operator_([](Vector r, Vector g){return r;}),
            terminate( CG::Termination::StrakosTichyEnergyError{} ),
              type_( type )
        {
            assert( type == "CG" || type == "RCG" || type == "TCG" || type == "TRCG" );
        }

        Solver::Solver(CallableOperator A, CallableOperator P,  MyOperator r_update, const std::string& type )
            : A_(std::move(A)), P_(std::move(P)), update_r_operator_(std::move(r_update)),
            terminate( CG::Termination::StrakosTichyEnergyError{} ), type_(type)
        {
            assert(type == "ICG");
        }

        Vector Solver::solve( const Vector& x, const Vector& b ) const
        {
            initializeRegularization();
            definiteness_ = DefiniteNess::PositiveDefinite;
            result = Result::Failed;

            terminate.set_eps( get( eps() ) );
            terminate.setRelativeAccuracy( get( getRelativeAccuracy() ) );
            terminate.setAbsoluteAccuracy( get( getAbsoluteAccuracy() ) );

            if ( type_ == "CG" || type_ == "TCG" )
                return cgLoop( x, b );
            else if(type_ == "ICG")
            {
                return icgLoop(x,b);
            }
            else
            {
                Vector y = x;
                while ( result != Result::Converged && result != Result::TruncatedAtNonConvexity )
                    y = cgLoop( x, b );
                return y;
            }
        }

        //    CG::TerminationCriterion& Solver::terminationCriterion() noexcept
        //    {
        //      return terminate;
        //    }

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

        const MyOperator& Solver::update_r_operator() const
        {
            return update_r_operator_;
        }

        Vector Solver::cgLoop( Vector x, Vector r ) const
        {
            terminate.clear();
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
                regularize( qAq, qPq );

                auto alpha = sigma / qAq;
                // if( getVerbosityLevel() > 1 ) std::cout << "    " << type_ << "  sigma = " << sigma << ", alpha = "
                // << alpha << ", qAq = " << qAq << ", qPq = " << qPq << std::endl;

                terminate.update( get( alpha ), get( qAq ), get( qPq ), get( sigma ) );

                //  don't trust small numbers
                if ( vanishingStep( step ) )
                {
                    if ( step == 1 )
                        x += q;
                    result = Result::Converged;
                    break;
                }

                auto Ax = A_( x );
                std::cout << "qAq/xAx: " << get( qAq ) << "/" << get( Ax( x ) ) << std::endl;
                if ( terminateOnNonconvexity( qAq, qPq, x, q, step ) )
                    break;

                x += ( get( alpha ) * q );

                // convergence test
                if ( terminate() )
                {
                    if ( verbose() )
                        std::cout << "    " << type_ << ": Terminating in iteration " << step << ".\n";
                    result = ( step == getMaxSteps() ) ? Result::Failed : Result::Converged;
                    break;
                }

                r -= alpha * Aq;
                adjustRegularizedResidual( alpha, Pq, r );

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

        Vector Solver::icgLoop(Vector x, Vector r) const
        {
            using SV = ::Spacy::Vector;
            // std::cout << "\t\t\t hello this is icgLoop !!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
            /**
                A_ is accessed by CG::Solver::A() and reflects H
                         / My  0   A^t \
                A_ = H = | 0   Mu -B^t |
                         \ A  -B   0   /

                     / My    0  \
                L =  |          |
                     \ 0     Mu /

                C =  [A  -B]

             From the paper :
                                 / L    \tilde{C}^T  \   / L*p_1 + \tilde{C}^T p_2   \   / L*p_1 \
                 \tilde{H} p =   |                   | = |                           | = |       | + w
                                 \ \tilde{C}    0    /   \ \tilde{C} p_1             /   \       /

             where
                     / 0 \tilde{C}^T \
                 w = |               | * p
                     \ \tilde{C}   0 /
             */

            auto H_tilde_operator = [this]( const SV& loc_p,
                                            const SV& loc_w ) -> SV {
                SV loc_p1 = primal_projection( loc_p );  // extract primal part st. there is no \tilde{C}^T*p_2
                // auto res = A()( loc_p1 );            // use the given operator
                SV res = A()( loc_p1 );                 // use the given operator
                res = primal_projection( res );          // delete the result of multiplication \tilde{C}*p_1
                // auto res = L_operator(loc_p);
                res += loc_w;
                // return cast_ref<SPV>(res);
                return res;
            };

            terminate.clear();
            result = Result::Failed;

            // auto Lp1 = L_operator( p1 );
            // w_{k-1}

            Real alpha = 0.0, beta = 0.0, sigma = 0.0, sigmaNew = 0.0;
            SV g, p(zero(x.space()));
            r -= A_( x );
            r *= -1;

            // p = - preconditioner^-1 * r;
            // p = -P_(r);
            p = -P_(r);
            g = p;

            SV  w  = zero(r.space()),
                Hp = zero(r.space());
            Real pHp = p( Hp );
            // SV Pp = P_( p );
            // Real pPp = p( Pp );
            
            // SV Pp = -r;  // required only for regularized or hybrid conjugate gradient methods
            
            sigma = abs(r( g )); // preconditioned residual norm squared

            // std::cout   << "\n sigma:\t"  << sigma << std::endl;

            // the conjugate gradient iteration on inexact kernel
            for ( unsigned step = 1; step < 50; step++ )
            {
                // std::cout << "\t loop: " << step << std::endl;

                w = get_w( r, g, beta, w );      // the heart of this algorithm
                Hp = H_tilde_operator( p, w ); // \tilde{H}*p_k
                pHp = p( Hp );
                // Real pPp = p( Pp );
                alpha = -get(r(p)) / get ( (pHp) );  // \alpha_k = \frac{- r_k^T p_k}{p_k^T \tilde{H} p_k}
                

                // std::cout   << " !!pHp:\t"      << pHp
                //             // << "\n !!pPp:\t"    << pPp
                //             << "\n rp:\t"  << get(r(p))
                            // << "\n alpha:\t"  << alpha 
                            // << "\n---------"
                            // << std::endl;

                terminate.update( get( alpha ), get( pHp ), 0, get( sigma ) );

                if(sigma==0)
                {
                    // std::cout << "sigma = 0" << std::endl;
                    break;
                }

                //  don't trust small numbers
                if ( vanishingStep( step ) )
                {
                    if ( step == 1 )
                        x += p;
                    result = Result::Converged;
                    std::cout<< "----------------------1" << std::endl;
                    break;
                }
                // if ( terminateOnNonconvexity( pHp, pPp, x, p, step ) )
                if ( terminateOnNonconvexity( pHp, 0, x, p, step ) )
                {
                    break;
                    std::cout<< "-----------------------2" << std::endl;
                }

                x += ( get( alpha ) * p );
                // x += get( alpha ) * Hp;

                // convergence test
                if ( terminate() )
                {
                    // std::cout<< "-----------------------3" << std::endl;
                    // std::cout   << "\t !!pHp:\t"    << pHp
                    //             // << "\n\t !!pPp:\t"    << pPp
                    //             // << "\n sigma:\t"  << sigma
                    //             << "\n\t alpha:\t"  << alpha << std::endl;
                    if ( verbose() )
                        std::cout << "    " << type_ << ": Terminating in iteration " << step << ".\n" << std::flush;
                    result = ( step == getMaxSteps() ) ? Result::Failed : Result::Converged;
                    break;
                }

                // r_next = r + get( alpha ) * Hp; // r_{k+1} = r_k + \alpha_k \tilde{H} p_k
                r += get( alpha ) * Hp; // r_{k+1} = r_k + \alpha_k \tilde{H} p_k

                // g_next = - preconditioner^-1 * r_next        // g_{k+1} = - Q^{-1} r_{k+1}
                g = - P_( r );
                
                // std::cout   << " r_next\t"    << norm(r_next)
                //             << "\n g_next\t"  << norm(g_next)
                //             // << "\n r-(g+)\t"  << r_next( g_next )
                //             << "\n x\t"       << norm(x) << std::endl;
                            
                // determine new search direction
                sigmaNew = abs(r( g ));
                beta = sigmaNew / sigma; // \beta_{k+1} = \frac{r_{k+1}^T g_{k+1}}{r_k^T g_k}
                sigma = sigmaNew;

                // p = g_next + get( beta ) * p;     // p_{k+1} = g_{k+1} + \beta_k p_k
                p *= get( beta );
                p += g;
                // Pp = r + get( beta ) * Pp;     // p_{k+1} = g_{k+1} + \beta_k p_k
                // Pp *= get( beta );
                // Pp += r; // Pq = r + beta*Pq
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
            if ( terminate.vanishingStep() )
            {
                if ( verbose() )
                    std::cout << "    " << type_
                              << ": Terminating due to numerically almost vanishing step in iteration " << step << "."
                              << std::endl;
                result = Result::Converged;
                return true;
            }
            return false;
        }

        bool Solver::terminateOnNonconvexity( Real qAq, Real qPq, Vector& x, const Vector& q, unsigned step ) const
        {
            if ( qAq > 0 )
                return false;
            if ( verbose() )
                std::cout << "    " << type_ << ": Negative curvature: " << qAq << std::endl;

            if ( type_ == "CG" )
            {
                if ( verbose() )
                {
                    std::cout << "    " << type_
                              << ": Direction of negative curvature encountered in standard CG Implementation!"
                              << std::endl;
                    std::cout << "    " << type_ << ": Either something is wrong with your operator or you should use "
                                                    "TCG, RCG or HCG. Terminating CG!"
                              << std::endl;
                }

                throw SingularOperatorException( "CG::terminateOnNonconvexity" );
            }

            if ( type_ == "TCG" || ( type_ == "TRCG" && terminate.minimalDecreaseAchieved() ) || type_ == "ICG" )
            {
                // At least do something to retain a little chance to get out of the nonconvexity. If a nonconvexity is
                // encountered in the first step something probably went wrong
                // elsewhere. Chances that a way out of the nonconvexity can be found are small in this case.
                if ( step == 1 )
                    x += q;
                if ( verbose() )
                    std::cout << "    " << type_ << ": Truncating at nonconvexity in iteration " << step << ": " << qAq
                              << std::endl;
                definiteness_ = DefiniteNess::Indefinite;
                result = Result::TruncatedAtNonConvexity;
                return true;
            }

            if ( type_ == "TRCG" || type_ == "RCG" )
            {
                updateRegularization( qAq, qPq );
                if ( verbose() )
                    std::cout << "    " << type_ << ": Regularizing at nonconvexity in iteration " << step << "."
                              << std::endl;
                definiteness_ = DefiniteNess::Indefinite;
                result = Result::EncounteredNonConvexity;
                return true;
            }

            assert( false );
            return false;
        }

        void Solver::initializeRegularization() const noexcept
        {
            if ( type_ == "CG" || type_ == "TCG" )
                return;
            theta = 0;
        }

        void Solver::regularize( Real& qAq, Real qPq ) const noexcept
        {
            if ( type_ == "CG" || type_ == "TCG" )
                return;
            qAq += theta * qPq;
        }

        void Solver::updateRegularization( Real qAq, Real qPq ) const
        {
            if ( type_ == "CG" || type_ == "TCG" )
                return;
            Real oldTheta = theta > 0 ? theta : Real( eps() );
            theta += ( 1 - qAq ) / abs( qPq );
            if ( getVerbosityLevel() > 1 )
                std::cout << "Computed regularization parameter: " << theta << std::endl;
            theta = min( max( minIncrease * oldTheta, theta ), maxIncrease * oldTheta );
            if ( getVerbosityLevel() > 1 )
                std::cout << "Updating regularization parameter from " << oldTheta << " to " << theta << std::endl;
        }

        void Solver::adjustRegularizedResidual( Real alpha, const Vector& Pq, Vector& r ) const
        {
            if ( type_ == "CG" || type_ == "TCG" )
                return;
            r -= ( alpha * theta ) * Pq;
        }

        Vector Solver::get_w(const ::Spacy::Vector& r, const ::Spacy::Vector& g, const Real& beta, ::Spacy::Vector w_old) const
        {
            // std::cout << "\t\t\t Hello there! This is get_w" << std::endl;

            // w_{k-1} = \beta * w_{k-1}
            w_old *= get(beta);
            // w_{k-1} = w_{k-1} - (r_y, B^T g_l, r_l)
            w_old -= update_r_operator_(r,g);

            return w_old;
        }
    }
}
