#pragma once

namespace Spacy
{
    /// @cond
    class Vector;
    /// @endcond

    /** @addtogroup ScalarGroup @{ */
    namespace Scalar
    {
        /**
         * @brief A linear solver for scalar problems.
         *
         * Computes \f$x/y\f$.
         */
        class LinearSolver
        {
        public:
            /// Construct linear solver for the computation of \f$\cdot/y\f$.
            explicit LinearSolver(double y);

            /// Compute \f$x/y\f$
            ::Spacy::Vector operator()(const ::Spacy::Vector& x) const;

        private:
            double y_;
        };
    }
    /** @} */
}
