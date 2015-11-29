// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_LIPSCHITZ_CONSTANT_HH
#define SPACY_LIPSCHITZ_CONSTANT_HH

#include "Spacy/Util/Mixins/eps.hh"
#include "Spacy/Spaces/RealSpace/real.hh"

namespace Spacy
{
  /**
   * @ingroup SpacyGroup
   * @brief A simple model for a Lipschitz constant \f$\omega\f$.
   */
  class LipschitzConstant : public Mixin::Eps
  {
  public:
    /**
     * @brief Construct Lipschitz constant.
     * @param initialOmega initial estimate for Lipschitz constant
     */
    LipschitzConstant(Real initialOmega = 1e-3);

    /**
     * \brief Assign new value to Lipschitz constant.
     *
     * If \f$\omega_{new}<0\f$ sets the Lipschitz constant to \f$minFactor * previousOmega\f$.
     * Restricts increase such that \f$\omega = min(\omega_{new},maxFactor * previousOmega)\f$.
     *
     * @param newOmega new Lipschitz constant
     */
    LipschitzConstant& operator=(Real newOmega);

    /**
     * @brief Access value of Lipschitz constant.
     * @return \f$\omega\f$
     */
    operator Real() const;

    Real operator()() const;

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
    Real omega_ = 1e-3, previousOmega_ = 1e-3, maxFactor_ = 1e6, minFactor_ = 1e-1;
  };
}

#endif // SPACY_LIPSCHITZ_CONSTANT_HH
