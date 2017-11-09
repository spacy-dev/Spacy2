#pragma once

#include <Spacy/Spaces/ScalarSpace/Real.hh>
#include <Spacy/vector.hh>

namespace Spacy
{
    namespace CG
    {
        /// Regularization for conjugate gradient methods.
        class Regularization
        {
        public:
            /// Initialize regularization.
            void init();

            /**
             * @brief Apply regularization to qAq, where q is the conjugate search direction.
             * @param qAq \f$ qAq \f$, where \f$q\f$ is the conjugate search direction and \f$A\f$
             * the system operator
             * @param qPq \f$ qPq \f$, where \f$q\f$ is the conjugate search direction and \f$P\f$
             * the preconditioner
             */
            void apply( Real& qAq, Real qPq ) const;

            /**
             * @brief Update regularization (parameter).
             * @param qAq \f$ qAq \f$, where \f$q\f$ is the conjugate search direction and \f$A\f$
             * the system operator
             * @param qPq \f$ qPq \f$, where \f$q\f$ is the conjugate search direction and \f$P\f$
             * the preconditioner
             */
            void update( Real qAq, Real qPq ) const;

            /**
             * @brief Adjust residual for consistency with the regularized left hand side.
             * @param alpha step length parameter of the conjugate gradient method
             * @param Pq \f$ Pq \f$, where \f$q\f$ is the conjugate search direction and \f$P\f$ the
             * preconditioner
             * @param r residual
             */
            void adjustResidual( Real alpha, const Vector& Pq, Vector& r ) const;
        };
    }
}
