#pragma once

#include <ostream>

#include <Spacy/Util/Mixins/eps.hh>
#include <Spacy/Util/Mixins/get.hh>
#include <Spacy/Spaces/RealSpace/real.hh>

namespace Spacy
{
    /// A simple model for a Lipschitz constant \f$\omega\f$.
    class LipschitzConstant :
            public Mixin::Get< double >,
            public Mixin::Eps
    {
    public:
        /**
         * @brief Construct Lipschitz constant.
         * @param initialOmega initial estimate for Lipschitz constant
         */
        explicit LipschitzConstant(double initialOmega = 1e-3);

        /**
         * \brief Assign new value to Lipschitz constant.
         *
         * If \f$\omega_{new}<0\f$ sets the Lipschitz constant to \f$minFactor * previousOmega\f$.
         * Restricts increase such that \f$\omega = min(\omega_{new},maxFactor * previousOmega)\f$.
         *
         * @param newOmega new Lipschitz constant
         */
        LipschitzConstant& operator=(double newOmega);

        /**
         * @brief Access previous value of Lipschitz constant.
         * @return \f$\omega_{k-1}\f$
         */
        double previous() const;

        /**
         * @brief Set allowed maximal relative increase.
         * @param factor new allowed maximal relative increase
         */
        void setMaxFactor(double factor);

        /**
         * @brief Set relative decrease for negative estimates.
         * @param factor new relative decrease for negative estimates of the Lipschitz constant
         */
        void setMinFactor(double factor);

    private:
        double previousOmega_ = 1e-3, maxFactor_ = 1e6, minFactor_ = 1e-3;
    };

    Real operator*(const LipschitzConstant& x, Real y);

    Real operator*(Real y, const LipschitzConstant& x);
}
