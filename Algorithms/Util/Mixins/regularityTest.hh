#ifndef ALGORITHM_UTIL_MIXIN_REGULARITY_TEST_HH
#define ALGORITHM_UTIL_MIXIN_REGULARITY_TEST_HH

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @brief Mixin class for maximal number of steps/iterations.
     */
    class RegularityTest
    {
    public:
      /**
       * @brief Constructor. Sets lower bound for regularity test.
       */
      explicit RegularityTest(double lowerBound = 1e-12) noexcept;

      /**
       * @brief Set lower bound of regularity test for termination criteria.
       */
      void setLowerBound(double lowerBound) noexcept;

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

    private:
        double lowerBound_ = 1e-12;
    };
  }
}
#endif // ALGORITHM_UTIL_MIXIN_REGULARITY_TEST_HH
