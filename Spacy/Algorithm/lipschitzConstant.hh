#ifndef ALGORITHM_LIPSCHITZ_CONSTANT_HH
#define ALGORITHM_LIPSCHITZ_CONSTANT_HH

#include "Spacy/Util/Mixins/eps.hh"

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
    LipschitzConstant(double initialOmega = 1e-3);

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
     * @brief Access value of Lipschitz constant.
     * @return \f$\omega\f$
     */
    operator double() const;

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
    double omega_ = 1e-3, previousOmega_ = 1e-3, maxFactor_ = 1e6, minFactor_ = 1e-1;
  };
}

#endif // ALGORITHM_LIPSCHITZ_CONSTANT_HH
