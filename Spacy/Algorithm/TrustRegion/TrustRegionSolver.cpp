#include "TrustRegionSolver.hh"

#include <Spacy/zeroVectorCreator.hh>
#include <Spacy/inducedScalarProduct.hh>
#include <Spacy/hilbertSpaceNorm.hh>
#include <Spacy/Util/log.hh>

namespace Spacy
{
    namespace Algorithm
    {
        DEFINE_LOG_TAG( static const char* log_tag = "TrustRegion" );

        TrustRegionSolver::TrustRegionSolver(C2Functional f)
            : f_(std::move(f))
        {
            set( [](const C2Functional& f, const Vector& x) -> Vector
            {
                auto solver = f.hessian(x).solver();
                return solver(-f.d1(x));
            } );

            set( [](Real radius, Real step_length, Real f_x, Real q_x) -> Real
            {
                if( abs(f_x - q_x) < step_length*step_length*step_length )
                    return min(1.0, 2*radius);

                return 0.5*radius;

            } );

            set( HilbertSpaceNorm(InducedScalarProduct(f_.hessian(zero(Space::R)))) );
        }

        void TrustRegionSolver::set(ComputeDescentDirection compute_descent_direction)
        {
            compute_descent_direction_ = std::move(compute_descent_direction);
        }

        void TrustRegionSolver::set(UpdateTrustRegionRadius update_trust_region_radius)
        {
            update_trust_region_radius_ = std::move(update_trust_region_radius);
        }

        void TrustRegionSolver::set(Norm norm)
        {
            norm_ = std::move(norm);
        }

        Vector TrustRegionSolver::operator()(Vector x)
        {
            LOG_SEPARATOR(log_tag);

            auto trust_region_radius = Real(1);
            for(auto i=0u; i<getMaxSteps(); ++i)
            {
                LOG(log_tag, "Iteration", i);

                auto dx = compute_descent_direction_(f_, x);
                dx *= get(trust_region_radius);
                auto q_x = f_(x) + f_.d1(x)(dx) + 0.5*f_.d2(x,dx)(dx);

                auto trial = x + dx;
                trust_region_radius = update_trust_region_radius_(trust_region_radius, norm_(dx), f_(trial), q_x);

                x += dx;
                LOG(log_tag, "|x|", norm_(x), "|dx|", norm_(dx), "trust_region", trust_region_radius);

                if( abs(trust_region_radius - 1) < eps() && norm_(dx) < getRelativeAccuracy() ) {
                    LOG_INFO(log_tag, "Converged");
                    return x;
                }

                LOG_SEPARATOR(log_tag);
            }

            LOG_INFO(log_tag, "Failed");
            return {};
        }
    }
}
