#ifndef SPACY_UTIL_MIXIN_REGULARITY_TEST_HH
#define SPACY_UTIL_MIXIN_REGULARITY_TEST_HH

#include "mixinConnection.hh"
#include "Spacy/Spaces/RealSpace/real.hh"

namespace Spacy
{
  namespace Mixin
  {
    /**
     * @ingroup MixinGroup
     * @brief %Mixin class for maximal number of steps/iterations.
     */
    class RegularityTest : public MixinConnection<RegularityTest>
    {
    public:
      /**
       * @brief Constructor.
       * @param lowerBound lower bound for regularity test
       */
      explicit RegularityTest(Real lowerBound = 1e-12) noexcept;

      /**
       * @brief Set lower bound of regularity test for termination criteria.
       * @param lowerBound new lower bound
       */
      void setLowerBound(Real lowerBound);

      /**
       * @brief Access lower bound.
       * @return lower bound
       */
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

      /**
       * @brief Attach RegularityTest.
       *
       * When setLowerBound(double lowerBound) is called, then also
       * other.setLowerBound(lowerBound) is invoked.
       */
      void attachRegularityTest(RegularityTest& other);

      /**
       * @brief Detach Eps before it gets deleted.
       */
      void detachRegularityTest(RegularityTest& other);

      /**
       * @brief update function for observer pattern.
       */
      void update(MixinConnection<RegularityTest>* changedSubject);

    private:
      Real lowerBound_;
    };
  }
}
#endif // SPACY_UTIL_MIXIN_REGULARITY_TEST_HH
