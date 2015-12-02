// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_ALGORITHM_NEWTON_DAMPING_STRATEGIES_HH
#define SPACY_ALGORITHM_NEWTON_DAMPING_STRATEGIES_HH

#include <functional>
#include "Spacy/vector.hh"
#include "Spacy/Spaces/RealSpace/real.hh"
#include "Spacy/Algorithm/dampingFactor.hh"
#include "Spacy/Util/Mixins/eps.hh"
#include "Spacy/Util/Mixins/regularityTest.hh"

namespace Spacy
{
  /// @cond
  class C1Operator;
  /// @endcond

  namespace Newton
  {
    namespace Damping
    {
      /**
       * @ingroup NewtonGroup
       * @brief Affine covariant damping strategy as described in @cite Deuflhard2004 [Sec. 3.3].
       */
      class AffineCovariant : public Mixin::Eps, public Mixin::RegularityTest
      {
      public:
        /// Constructor.
        AffineCovariant(const C1Operator& F);

        /// Compute damping factor.
        DampingFactor operator()(const std::function<Vector(const Vector&)>& DFInv_, const Vector& x, const Vector& dx) const;

      private:
        const C1Operator& F_;

        mutable DampingFactor oldNu = -1;
        mutable Real normOldDx = Real{-1}, normOldDs = Real{-1};
        mutable Vector oldDs;
      };

      /**
       * @ingroup NewtonGroup
       * @brief Affine contravariant damping strategy as described in @cite Deuflhard2004 [Sec. 3.2].
       */
      class AffineContravariant : public Mixin::Eps, public Mixin::RegularityTest
      {
      public:
        /// Constructor.
        AffineContravariant(const C1Operator& F);

        /// Compute damping factor.
        DampingFactor operator()(const std::function<Vector(const Vector&)>&, const Vector& x, const Vector& dx) const;

      private:
        const C1Operator& F_;
        mutable Real muPrime = Real{-1}, norm_F_x_old = Real{-1};
      };

      /**
       * @ingroup NewtonGroup
       * @brief No damping, yields local newton method.
       */
      class None
      {
      public:
        /// Constructor.
        None(const C1Operator&);

        /**
         * @brief Compute damping factor.
         * @return 1
         */
        DampingFactor operator()(const std::function<Vector(const Vector&)>&, const Vector&, const Vector&) const;
      };
    }
  }
}

#endif // SPACY_ALGORITHM_NEWTON_DAMPING_STRATEGIES_HH
