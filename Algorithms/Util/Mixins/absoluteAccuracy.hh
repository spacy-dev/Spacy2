#ifndef ALGORITHM_UTIL_MIXIN_ABSOLUTE_ACCURACY_HH
#define ALGORITHM_UTIL_MIXIN_ABSOLUTE_ACCURACY_HH

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @brief Mixin class for absolute accuracy.
     */
    class AbsoluteAccuracy
    {
    public:
      /**
       * @brief Constructor. Sets absolute accuracy.
       */
      explicit AbsoluteAccuracy(double absoluteAccuracy = 1e-12);

      /**
       * @brief Set absolute accuracy.
       */
      void setAbsoluteAccuracy(double) noexcept;

      /**
       * @brief Get absolute accuracy.
       */
      double absoluteAccuracy() const noexcept;

    private:
      double absoluteAccuracy_ = 1e-12;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_ABSOLUTE_ACCURACY_HH
