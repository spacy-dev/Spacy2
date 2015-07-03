#ifndef ALGORITHM_UTIL_MIXIN_RELATIVE_ACCURACY_HH
#define ALGORITHM_UTIL_MIXIN_RELATIVE_ACCURACY_HH

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @brief Mixin class for relative accuracy.
     */
    class RelativeAccuracy
    {
    public:
      /**
       * @brief Constructor. Sets relative accuracy.
       */
      explicit RelativeAccuracy(double relativeAccuracy = 1e-12);

      /**
       * @brief Set relative accuracy.
       */
      void setRelativeAccuracy(double) noexcept;

      /**
       * @brief Get relative accuracy.
       */
      double relativeAccuracy() const noexcept;

    private:
      double relativeAccuracy_ = 1e-12;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_RELATIVE_ACCURACY_HH
