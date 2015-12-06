// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_DAMPING_FACTOR_HH
#define SPACY_DAMPING_FACTOR_HH

#include <ostream>
#include "Spacy/Util/Mixins/eps.hh"

namespace Spacy
{
  /// @cond
  class Real;
  /// @endcond

  /// A simple model of a damping factor \f$\nu\f$ that is computed up to a prescribed accuracy \f$\varepsilon\f$.
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

    /// Access damping factor \f$\nu\f$.
    operator double() const;

    /// Access damping factor \f$\nu\f$.
    operator Real() const;

  private:
    void set(double nu);

    double nu_ = 1;
  };

  std::ostream& operator<<(std::ostream& os, DampingFactor nu);
}

#endif // SPACY_DAMPING_FACTOR_HH
