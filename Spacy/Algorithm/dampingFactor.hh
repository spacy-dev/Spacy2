#ifndef SPACY_DAMPING_FACTOR_HH
#define SPACY_DAMPING_FACTOR_HH

#include "Spacy/Util/Mixins/eps.hh"
#include "Spacy/Spaces/RealSpace/real.hh"

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
    DampingFactor(Real nu, double eps = 1e-3);

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
    DampingFactor& operator=(Real nu);

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
    operator Real() const;

    /**
     * @brief Access damping factor
     * @return \f$\nu\f$
     */
    Real operator()() const;

  private:
    void set(Real nu);

    Real nu_ = 1;
  };
}

#endif // SPACY_DAMPING_FACTOR_HH
