#pragma once

#include <Spacy/Util/Mixins/eps.hh>
#include <Spacy/Util/Mixins/get.hh>
#include <Spacy/Spaces/ScalarSpace/real.hh>

namespace Spacy
{
    /// A simple model of a damping factor \f$\nu\f$ that is computed up to a prescribed accuracy \f$\varepsilon\f$.
    class DampingFactor :
            public Mixin::Get<Real>,
            public Mixin::Eps
    {
    public:
        /**
         * @brief Constructor.
         * @param nu damping factor \f$\nu\f$
         * @param eps accuracy \f$\varepsilon\f$.
         */
        explicit DampingFactor(Real nu, Real eps = 1e-3) noexcept;

        /**
         * @brief Set damping factor \f$\nu\f$.
         *
         * If \f$ |\nu-1| < \varepsilon \f$ then \f$\nu\f$ is set to 1.
         *
         * @param nu damping factor
         */
        DampingFactor& operator=(Real nu) noexcept;

        /// In-place multiplication
        DampingFactor& operator*=(Real value);

        /// Invert damping factor
        DampingFactor operator-() const noexcept;
    };
}
