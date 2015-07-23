#ifndef ALGORITHM_UTIL_MIXIN_MINIMAL_ACCURACY_HH
#define ALGORITHM_UTIL_MIXIN_MINIMAL_ACCURACY_HH

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @brief Mixin class for minimal accuracy.
     */
    class MinimalAccuracy
    {
    public:
      /**
       * @brief Constructor. Sets minimal accuracy.
       */
      explicit MinimalAccuracy(double minimalAccuracy = 0.25) noexcept;

      /**
       * @brief Set minimal accuracy.
       */
      void setMinimalAccuracy(double) noexcept;

      /**
       * @brief Get minimal accuracy.
       */
      double minimalAccuracy() const noexcept;

    private:
      double minimalAccuracy_ = 0.25;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_MINIMAL_ACCURACY_HH
