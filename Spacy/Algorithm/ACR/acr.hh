#pragma once

#include <chrono>

#include <Spacy/linearSolver.hh>
#include <Spacy/c2Functional.hh>
#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>
#include <Spacy/Util/mixins.hh>
#include <Spacy/Spaces/ScalarSpace/real.hh>

namespace Spacy
{
    /// @cond
    namespace CompositeStep { class CubicModel; }
    /// @endcond

    namespace ACR
    {
        /**
         * @brief An ACR solver for solving unconstrained minimization problems
         *
         * An ACR solver for solving problems of the form
         * \f[\min f(x).\f]
         */
        class ACRSolver :
                public Mixin::RegularityTest ,
                public Mixin::Timer<std::chrono::milliseconds> ,
                public Mixin::DecreaseCondition,
                public Mixin::Eps,
                public Mixin::MaxSteps,
                public Mixin::RelativeAccuracy,
                public Mixin::Verbosity
        {
            enum class StepMonitor { Rejected , Accepted };

        public:
            /**
             * @brief Constructor.
             * @param f functional to minimize
             */
            ACRSolver(C2Functional f, double eta1 = 0.25, double eta2 = 0.5, double epsilon = 1e-4, double relativeAccuracy = 1e-4, double omegaMax = 1e8);

            /// Compute solution starting at \f$x_0=0\f$.
            Vector operator()();

            /**
             * @brief Compute solution.
             * @param x0 initial iterate
             */
            Vector operator()(const Vector& x0);

        private:
            Vector computeStep(const Vector& x) const;

            StepMonitor acceptanceTest(const Vector& x, const Vector& dx, const Real& lambda, const CompositeStep::CubicModel& cubicModel);

            Real weightChange(::Spacy::Real omega) const;

            C2Functional f_;
            const VectorSpace& domain_;
            StepMonitor stepMonitor = StepMonitor::Accepted;

            const double eta1_, eta2_, epsilon_, omegaMax_;
            Real rho_ = 1;
            Real omega_ = 1e-6;
        };
    }
}

