#ifndef ALGORITHM_UTIL_MIXIN_REGULARITY_TEST_HH
#define ALGORITHM_UTIL_MIXIN_REGULARITY_TEST_HH

#include "mixinConnection.hh"

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @ingroup MixinGroup
     * @brief Mixin class for maximal number of steps/iterations.
     */
    class RegularityTest : public MixinConnection<RegularityTest>
    {
    public:
      /**
       * @brief Constructor. Sets lower bound for regularity test.
       */
      explicit RegularityTest(double lowerBound = 1e-12) noexcept;

      /**
       * @brief Set lower bound of regularity test for termination criteria.
       */
      void setLowerBound(double lowerBound);

      /**
       * @brief Get lower bound.
       */
      double lowerBound() const noexcept;

      /**
       * @brief Apply regularity test.
       * @return \f$nu > lowerBound_\f$
       */
      bool regularityTestPassed(double nu) const noexcept;

      /**
       * @brief Apply regularity test.
       * @return \f$nu <= lowerBound_\f$
       */
      bool regularityTestFailed(double nu) const noexcept;

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

      void update(MixinConnection<RegularityTest>* changedSubject);

    private:
      double lowerBound_ = 1e-12;
    };
  }
}
#endif // ALGORITHM_UTIL_MIXIN_REGULARITY_TEST_HH
