#pragma once

#include <Spacy/Spaces/RealSpace/real.hh>
#include <Spacy/Util/Mixins/accuracy.hh>
#include <Spacy/Util/Mixins/verbosity.hh>
#include <Spacy/Util/Mixins/eps.hh>
#include <Spacy/Util/log.hh>

namespace Spacy
{
    /// @cond
    class DampingFactor;
    class C1Operator;
    class Vector;
    /// @endcond

    namespace Newton
    {
        namespace Termination
        {
            DEFINE_LOG_TAG( static const char* covariant_log_tag = "CovariantTerminationStrategy" )
            DEFINE_LOG_TAG( static const char* contravariant_log_tag = "ContravariantTerminationStrategy" )

            /**
             * @ingroup NewtonGroup
             * @brief Affine covariant relative error criterion.
             */
            class AffineCovariant :
                    public Mixin::RelativeAccuracy,
                    public Mixin::Eps
            {
            public:
                /**
                 * @brief Constuctor.
                 */
                AffineCovariant(const C1Operator&, Real relativeAccuracy, Real eps);

                /**
                 * @brief Apply termination criterion.
                 * @param nu damping factor
                 * @param x iterate
                 * @param dx correction
                 * @return true if \f$\nu=1\f$ and \f$ \|dx\|<rel_acc\|x\| \f$ or \f$\|x\|=\|dx\|=0\f$, else false
                 */
                bool operator()(DampingFactor nu, const Vector& x, const Vector& dx) const;

            private:
                mutable bool beforeFirstIteration_ = true;
            };

            /**
             * @ingroup NewtonGroup
             * @brief Affine contravariant relative error criterion.
             */
            class AffineContravariant :
                    public Mixin::RelativeAccuracy,
                    public Mixin::Eps
            {
            public:
                /**
                 * @brief Constructor.
                 */
                AffineContravariant(const C1Operator& F, Real relativeAccuracy, Real eps);

                /**
                 * @brief Apply termination criterion.
                 * @param nu damping factor
                 * @param x iterate
                 * @return true if \f$\nu=1\f$ and \f$ \|F(x)\|<rel_acc\|F(x_0)\| \f$, else false
                 */
                bool operator()(DampingFactor nu, const Vector& x, const Vector&) const;

            private:
                const C1Operator& F_;
                mutable Real initialResidual = Real{-1};
            };
        }
    }
}
