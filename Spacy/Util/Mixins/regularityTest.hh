#pragma once

#include <Spacy/Algorithm/dampingFactor.hh>

#include "mixinConnection.hh"

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
      explicit RegularityTest(DampingFactor lowerBound = DampingFactor{1e-12}) noexcept;

      /// Set lower bound of regularity test for termination criteria.
      void setLowerBound(DampingFactor lowerBound);

      DampingFactor getLowerBound() const noexcept;

      /**
       * @brief Apply regularity test.
       * @param nu damping factor
       * @return \f$nu > lowerBound_\f$
       */
      bool regularityTestPassed(DampingFactor nu) const noexcept;

      /**
       * @brief Apply regularity test.
       * @param nu damping factor
       * @return \f$nu <= lowerBound_\f$
       */
      bool regularityTestFailed(DampingFactor nu) const noexcept;

      /// update function for observer pattern.
      void update(RegularityTest* changedSubject);

    private:
      DampingFactor lowerBound_;
    };
  }
}
