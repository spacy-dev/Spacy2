#ifndef ALGORITHM_LIPSCHITZ_CONSTANT_HH
#define ALGORITHM_LIPSCHITZ_CONSTANT_HH

#include "Util/Mixins/eps.hh"

namespace Algorithm
{
  /**
   * @ingroup AlgorithmGroup
   * @brief A simple model for Lipschitz constants.
   */
  class LipschitzConstant : public Mixin::Eps
  {
  public:
    /// Construct Lipschitz constant.
    LipschitzConstant(double initialOmega = 1e-3);

    /**
     * \brief Assign new value to Lipschitz constant.
     *
     * If \f$\omega_{new}<0\f$ sets the Lipschitz constant to \f$minFactor * previousOmega\f$.
     * Restricts increase such that \f$\omega = min(\omega_{new},maxFactor * previousOmega)\f$.
     */
    LipschitzConstant& operator=(double newOmega);

    /// Access value of Lipschitz constant.
    operator double() const;

    /// Access previous value of Lipschitz constant.
    double previous() const;

    /// Set allowed maximal relative increase.
    void setMaxFactor(double factor);

    /// Set relative decrease for negative estimates.
    void setMinFactor(double factor);

  private:
    double omega_ = 1e-3, previousOmega_ = 1e-3, maxFactor_ = 1e6, minFactor_ = 1e-1;
  };
}

#endif // ALGORITHM_LIPSCHITZ_CONSTANT_HH
