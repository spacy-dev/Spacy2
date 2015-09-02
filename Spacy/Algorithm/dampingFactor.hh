#ifndef ALGORITHM_DAMPING_FACTOR_HH
#define ALGORITHM_DAMPING_FACTOR_HH

#include "Spacy/Util/Mixins/eps.hh"

namespace Spacy
{
  /**
   * @ingroup SpacyGroup
   * @brief A simple model of a damping factor \f$\nu\f$ that is computed up to a prescribed accuracy \f$\varepsilon\f$.
   */
  class DampingFactor : public Mixin::Eps
  {
  public:
    /**
     * @brief Constructor.
     * @param nu damping factor \f$\nu\f$
     * @param eps accuracy \f$\varepsilon\f$.
     */
    DampingFactor(double nu, double eps = 1e-3);

    /**
     * @brief Set damping factor \f$\nu\f$.
     *
     * If \f$ |\nu-1| < \varepsilon \f$ then \f$\nu\f$ is set to 1.
     *
     * @param nu damping factor
     */
    DampingFactor& operator=(double nu);

    /**
     * @brief Access damping factor
     * @return \f$\nu\f$
     */
    operator double() const;

  private:
    void set(double nu);

    double nu_ = 1;
  };
}

#endif // ALGORITHM_DAMPING_FACTOR_HH
