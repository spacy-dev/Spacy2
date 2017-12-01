#include "newton.hh"

#include <Spacy/Algorithm/Newton/dampingStrategies.hh>
#include <Spacy/Algorithm/Newton/terminationCriteria.hh>
#include <Spacy/Util/Exceptions/notConvergedException.hh>
#include <Spacy/Util/log.hh>

#include <Spacy/c1Operator.hh>
#include <Spacy/derivative.hh>
#include <Spacy/vectorSpace.hh>
#include <Spacy/zeroVectorCreator.hh>

#include <Spacy/Util/cast.hh>

namespace Spacy
{
    namespace Newton
    {
        Vector newton(
            const C1Operator& F, const Vector& x0,
            const std::function< DampingFactor( const std::function< Vector( const Vector& ) >&,
                                                const Vector&, const Vector& ) >& dampingStrategy,
            const std::function< bool( DampingFactor, const Vector&, const Vector& ) >&
                terminationCriterion,
            const std::function< bool( const Vector&, const Vector& ) >& errorEstimator,
            const Parameter& p )
        {
            LOG_SEPARATOR_F;
            p.startTimer();

            auto x = x0;
            auto nu = DampingFactor{1};
            if ( terminationCriterion( nu, x, x ) )
                return x;

            for ( unsigned i = 1; i <= p.getMaxSteps(); ++i )
            {
                LOG_F( "Iteration", i )

                auto dF_inv = d1( F, x ) ^ -1;

                auto dx = dF_inv( -F( x ) );
                nu = dampingStrategy( dF_inv, x, dx );
                x += nu * dx;

                LOG_F( "damping factor", nu )
                LOG_F( "|x|", norm( x ), "|dx|", norm( dx ) )

                if ( nu == 1 )
                {
                    if ( errorEstimator && errorEstimator( x, dx ) )
                    {
                        LOG_INFO_F( "Grid refinement." )
                        continue;
                    }
                }

                if ( terminationCriterion( nu, x, dx ) )
                {
                    LOG_INFO_F( "Converged.\n" )
                    LOG_F( "computation time (in ms): ", p.elapsedTime() )
                    return x;
                }
                LOG_SEPARATOR_F;
            }

            throw Exception::NotConverged( "Newton" );
        }
    }

    Vector localNewton( const C1Operator& F, const Vector& x0, const Spacy::Newton::Parameter& p )
    {
        return Newton::newton< Newton::Damping::None, Newton::Termination::AffineCovariant >( F, x0,
                                                                                              p );
    }

    Vector localNewton( const C1Operator& F, const Newton::Parameter& p )
    {
        return localNewton( F, zero( F.domain() ), p );
    }

    Vector
    covariantNewton( const C1Operator& F, const Vector& x0, const Spacy::Newton::Parameter& p,
                     const std::function< bool( const Vector&, const Vector& ) >& errorEstimator )
    {
        return Newton::newton< Newton::Damping::AffineCovariant,
                               Newton::Termination::AffineCovariant >( F, x0, p, errorEstimator );
    }

    Vector
    covariantNewton( const C1Operator& F, const Spacy::Newton::Parameter& p,
                     const std::function< bool( const Vector&, const Vector& ) >& errorEstimator )
    {
        return covariantNewton( F, zero( F.domain() ), p, errorEstimator );
    }

    Vector contravariantNewton( const C1Operator& F, const Vector& x0,
                                const Spacy::Newton::Parameter& p )
    {
        return Newton::newton< Newton::Damping::AffineContravariant,
                               Newton::Termination::AffineContravariant >( F, x0, p );
    }

    Vector contravariantNewton( const C1Operator& F, const Spacy::Newton::Parameter& p )
    {
        return contravariantNewton( F, zero( F.domain() ), p );
    }
}
