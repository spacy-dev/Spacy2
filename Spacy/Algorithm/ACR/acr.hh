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
         * @brief Adaptive cubic regularization approach for solving unconstrained minimization problems
         * Compute direction of descent with one Newton step
         * Use a cubic error model to obtain acceptable corrections 
         * Solve problems of the form
         * \f[\min f(x).\f]
         */
        class ACRSolver :
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
            ACRSolver(C2Functional f, double eta1 = 0.25, double eta2 = 0.5, double epsilon = 1e-4, double relativeAccuracy = 1e-4, double omegaMax = 1e8,  double lambdaMax = 2e-2);

            /// Compute solution starting at \f$x_0=0\f$.
            Vector operator()();

            /**
             * @brief Compute solution.
             * @param x0 initial iterate
             */
            Vector operator()(const Vector& x0);

        private:
            
            /**
             * @brief Compute correction dx.
             * @param x current iterate
             */
            Vector computeStep(const Vector& x) const;

            /**
             * @brief Test if dx is an acceptable correction.
             * @param x current iterate
             * @param dx correction
             * @param lambda damping factor
             * @param cubicModel cubic model
             */
            StepMonitor acceptanceTest(const Vector& x, const Vector& dx, const Real& lambda, const CompositeStep::CubicModel& cubicModel);

            /**
             * @brief Update the weight Parameter of the cubic model.
             * @param omega weight Parameter of the cubic model
             */
            Real weightChange(::Spacy::Real omega) const;

            C2Functional f_;
            const VectorSpace& domain_;
            StepMonitor stepMonitor = StepMonitor::Accepted;
            
            // epsilon_ termination criterion
            const double eta1_, eta2_, epsilon_, omegaMax_,  lambdaMax_;
            Real rho_ = 1;
            Real omega_ = 1e-6;
        };
    }
}

