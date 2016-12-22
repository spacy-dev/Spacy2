#pragma once

namespace Spacy
{
    namespace CG
    {
        /// Type-erased termination criterion for conjugate gradient methods.
        class TerminationCriterion
        {
        public:
            bool operator()() const;

            void clear();

            void update(double alpha, double qAq, double qPq, double rPINVr);

            bool vanishingStep() const;

            bool minimalDecreaseAchieved() const;

            void set_eps(double eps);

            void setAbsoluteAccuracy(double accuracy);

            void setMinimalAccuracy(double accuracy);

            void setRelativeAccuracy(double accuracy);
        };
    }
}

