#pragma once

#include <Spacy/Spaces/ScalarSpace/real.hh>

namespace Spacy
{
    namespace Scalar
    {
        /// A one-dimensional cubic function \f$q(t) = a + bt + ct^2 + dt^3\f$.
        class Cubic
        {
        public:
            Cubic(Real a, Real b, Real c, Real d) noexcept;

            /// Compute \f$q(t) = a + bt + ct^2 + dt^3 \f$.
            Real operator()(Real t) const noexcept;

        private:
            Real a_, b_, c_, d_;
        };
    }
}
