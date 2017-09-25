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

        bool Solver::indefiniteOperator() const noexcept
        {
            return definiteness_ == DefiniteNess::Indefinite;
        }

        SV Solver::solve( SV x, const SV& b ) const
        {
            /**
                for k=1...
                    Solve with inexact cg:
                                    / y'      \   / c_y \
                        \tilde{H} * | u'      | + | c_u | = 0
                                    \ \lambda /   \ 0   /
                    calc
                        \delta_u = u' - u_k
                    and solve with a direct solver:
                        A \delta_y - B \delta_u = 0
                    update variables
                        y_{k+1} = y_k + \delta_y
                        u_{k+1} = u_k + \delta_y
            */

            assert(is<SPV>(x));
            assert(is<SPV>(b));


            // set up termination criterium
            definiteness_ = DefiniteNess::PositiveDefinite;
            result = Result::Failed;

            terminate.set_eps( get( eps() ) );
            terminate.setRelativeAccuracy( get( getRelativeAccuracy() ) );
            terminate.setAbsoluteAccuracy( get( getAbsoluteAccuracy() ) );

            terminate.clear();
            result = Result::Failed;

            
            SV dx( x ), r; //step
            Real qAq = 0, alpha = 0, sigma = 0; // values for StrakosTichy convergence test


            for ( int step = 0; step < 10; ++step )
            {
                std::cout << "\t\t\t\t\t\tausserer loop: " << step << std::endl;
                // solve on inexact kernel
                dx = icg.solve( x, b );

                // solve projection on actual kernel
                dx = projection_operator( x, dx );
                // up to this point dx actually stores the intermediate position not any kind of difference
                // update lagrange or project it away
                if(!is_update_lagrange_multiplier)
                    dx = primal_projection(dx);

                if(is_calc_dampening)
                    dx *= get(get_dampening_on_kernel(x, b, dx));

                // all necessary values for StrakosTichyEnergyError
                qAq = dx(A_(dx));
                r = A_(x) - b;
                sigma = r( P_(r) );
                alpha = sigma / qAq;

                terminate.update( get( alpha ), get( qAq ), 0, get( sigma ) );

                //  don't trust small numbers
                if ( vanishingStep( step ) )
                {
                    if ( step == 1 )
                        x += dx;
                    result = Result::Converged;
                    break;
                }

                // should never be encountered ?
                if ( terminateOnNonconvexity( qAq, 0, x, dx, step ) )
                    break;

                // go step
                x += dx;

                // convergence test
                if ( terminate() )
                {
                    if ( verbose() )
                        std::cout << "    PPCG: Terminating in iteration " << step << ".\n";
                    result = ( step == getMaxSteps() ) ? Result::Failed : Result::Converged;
                    break;
                }

                //update lagrange update
                if(is_update_lagrange_multiplier)
                    x = update_lagrange(x, b);

            }
            return x;
        }

        Real Solver::get_dampening_on_kernel(const SV& x, const SV& b, const SV& dx) const
        {
            // w = - \frac{Lx+b)^T dx}{dx^T Ldx}
            if (get(dx(dx)) == 0)
                return 1.0;
            
            // std::cout   << "\n----"
            //             << "\nx(x): "    << get(x(x))
            //             << "\tb(b): "    << get(b(b))
            //             << "\tdx(dx): "  << get(dx(dx))
            //             << std::endl;

            Real res = 0.0;
            SV Lx  = primal_projection(A_(primal_projection(x)));
            SV Ldx = primal_projection(A_(primal_projection(dx)));
            // double numerator = get(dx(Lx+b));
            // double denominator = get(dx(Ldx));
            // std::cout << "dämpfung: " << denominator << " / " << numerator << std::endl;
            return -1.0*( (dx(Lx+b)) / (dx(Ldx)) );
        }
        bool Solver::vanishingStep( unsigned step ) const
        {
            if ( terminate.vanishingStep() )
            {
                if ( verbose() )
                    std::cout << "    PPCG"
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
                std::cout << "    PPCG: Negative curvature: " << qAq << std::endl;

            /*
                how to determine nonconvexity
            if ( type_ == "CG" || type_ == "ICG" )
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

            if ( type_ == "TCG" || ( type_ == "TRCG" && terminate.minimalDecreaseAchieved() ) )
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
            */
            std::cout << "qAq: " << qAq << std::endl;
            assert( false );
            return false;
        }
    }
}