#pragma once

#include <Spacy/Util/Mixins/Eps.hh>
#include <Spacy/Util/Mixins/Get.hh>
#include <Spacy/Spaces/ScalarSpace/Real.hh>

namespace Spacy
{
    /// A simple model for a Lipschitz constant \f$\omega\f$.
    class LipschitzConstant :
            public Mixin::Get<Real>,
            public Mixin::Eps
    {
    public:
        /**
         * @brief Construct Lipschitz constant.
         * @param initialOmega initial estimate for Lipschitz constant
         */
        explicit LipschitzConstant(Real initialOmega = 1e-3);

        /**
         * @brief Assign new value to Lipschitz constant.
         *
         * If \f$\omega_{new}<0\f$ sets the Lipschitz constant to \f$minFactor * previousOmega\f$.
         * Restricts increase such that \f$\omega = min(\omega_{new},maxFactor * previousOmega)\f$.
         *
         * @param newOmega new Lipschitz constant
         */
        LipschitzConstant& operator=(Real newOmega);

        /**
         * @brief Access previous value of Lipschitz constant.
         * @return \f$\omega_{k-1}\f$
         */
        Real previous() const;

        /**
         * @brief Set allowed maximal relative increase.
         * @param factor new allowed maximal relative increase
         */
        void setMaxFactor(Real factor);

        /**
         * @brief Set relative decrease for negative estimates.
         * @param factor new relative decrease for negative estimates of the Lipschitz constant
         */
        void setMinFactor(Real factor);

    private:
        Real previousOmega_ = 1e-3, maxFactor_ = 1e6, minFactor_ = 1e-3;
    };

    Real operator*(const LipschitzConstant& x, Real y);

    Real operator*(Real x, const LipschitzConstant& y);
}
