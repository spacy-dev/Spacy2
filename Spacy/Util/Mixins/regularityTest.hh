// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_UTIL_MIXIN_REGULARITY_TEST_HH
#define SPACY_UTIL_MIXIN_REGULARITY_TEST_HH

#include "mixinConnection.hh"
#include "Spacy/Spaces/RealSpace/real.hh"

namespace Spacy
{
  namespace Mixin
  {
    /// %Mixin class for maximal number of steps/iterations.
    class RegularityTest : public MixinConnection<RegularityTest>
    {
    public:
      /**
       * @brief Constructor.
       * @param lowerBound lower bound for regularity test
       */
      explicit RegularityTest(Real lowerBound = 1e-12) noexcept;

      /// Set lower bound of regularity test for termination criteria.
      void setLowerBound(Real lowerBound);

      Real lowerBound() const noexcept;

      /**
       * @brief Apply regularity test.
       * @param nu damping factor
       * @return \f$nu > lowerBound_\f$
       */
      bool regularityTestPassed(Real nu) const noexcept;

      /**
       * @brief Apply regularity test.
       * @param nu damping factor
       * @return \f$nu <= lowerBound_\f$
       */
      bool regularityTestFailed(Real nu) const noexcept;

      /// update function for observer pattern.
      void update(MixinConnection<RegularityTest>* changedSubject);

    private:
      Real lowerBound_;
    };
  }
}
#endif // SPACY_UTIL_MIXIN_REGULARITY_TEST_HH
