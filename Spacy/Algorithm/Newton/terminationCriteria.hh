// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_ALGORITHM_NEWTON_TERMINATION_CRITERIA_HH
#define SPACY_ALGORITHM_NEWTON_TERMINATION_CRITERIA_HH

#include "Spacy/Spaces/RealSpace/real.hh"
#include "Spacy/Util/Mixins/accuracy.hh"
#include "Spacy/Util/Mixins/verbosity.hh"
#include "Spacy/Util/Mixins/eps.hh"

namespace Spacy
{
  /// @cond
  class DampingFactor;
  class C1Operator;
  class Vector;
  /// @endcond

  namespace Newton
  {
    namespace Termination
    {
      /**
       * @ingroup NewtonGroup
       * @brief Affine covariant relative error criterion.
       */
      class AffineCovariant :
          public Mixin::RelativeAccuracy,
          public Mixin::Verbosity,
          public Mixin::Eps
      {
      public:
        /**
         * @brief Constuctor.
         */
        AffineCovariant(const C1Operator&, double relativeAccuracy, bool verbose = false);

        /**
         * @brief Apply termination criterion.
         * @param nu damping factor
         * @param x iterate
         * @param dx correction
         * @return true if \f$\nu=1\f$ and \f$ \|dx\|<rel_acc\|x\| \f$ or \f$\|x\|=\|dx\|=0\f$, else false
         */
        bool operator()(DampingFactor nu, const Vector& x, const Vector& dx) const;

      private:
        mutable bool beforeFirstIteration_ = true;
      };

      /**
       * @ingroup NewtonGroup
       * @brief Affine contravariant relative error criterion.
       */
      class AffineContravariant :
          public Mixin::RelativeAccuracy,
          public Mixin::Verbosity,
          public Mixin::Eps
      {
      public:
        /**
         * @brief Constructor.
         */
        AffineContravariant(const C1Operator& F, double relativeAccuracy, bool verbose = false);

        /**
         * @brief Apply termination criterion.
         * @param nu damping factor
         * @param x iterate
         * @return true if \f$\nu=1\f$ and \f$ \|F(x)\|<rel_acc\|F(x_0)\| \f$, else false
         */
        bool operator()(DampingFactor nu, const Vector& x, const Vector&) const;

      private:
        const C1Operator& F_;
        mutable Real initialResidual = Real{-1};
      };
    }
  }
}

#endif // SPACY_ALGORITHM_NEWTON_TERMINATION_CRITERIA_HH
