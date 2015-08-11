#ifndef ALGORITHM_DAMPING_FACTOR_HH
#define ALGORITHM_DAMPING_FACTOR_HH

#include "vector.hh"
#include "Util/Mixins/eps.hh"

namespace Algorithm
{
  class Vector;

  /**
   * @brief A simple model of a damping factor \f$\nu\f$ that is computed up to a prescribed accuracy \f$\varepsilon\f$.
   */
  class DampingFactor : public Mixin::Eps
  {
  public:
    /**
     * @brief Constructor. Sets damping factor \f$\nu\f$ and accuracy \f$\varepsilon\f$.
     */
    DampingFactor(double nu, double eps);

    /**
     * @brief Constructor. Sets damping factor \f$\nu\f$. The accuracy is set to \f$1e-3\f$.
     */
    DampingFactor(double nu = 1);

    /**
     * @brief Set damping factor \f$\nu\f$.
     *
     * If \f$ |\nu-1| < \varepsilon \f$ then \f$\nu\f$ is set to 1.
     */
    DampingFactor& operator=(double);

    /// Access damping factor
    operator double() const;

  private:
    void set(double nu);

    double nu_ = 1;
  };
}

#endif // ALGORITHM_DAMPING_FACTOR_HH
