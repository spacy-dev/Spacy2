#ifndef ALGORITHM_UTIL_MIXIN_ABSOLUTE_ACCURACY_HH
#define ALGORITHM_UTIL_MIXIN_ABSOLUTE_ACCURACY_HH

#include "Util/DesignPatterns/observer.hh"

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @brief Mixin class for absolute accuracy.
     */
    class AbsoluteAccuracy : public DesignPattern::Observer::Subject , public DesignPattern::Observer::Observer
    {
    public:
      /**
       * @brief Constructor. Sets absolute accuracy.
       */
      explicit AbsoluteAccuracy(double absoluteAccuracy = 1e-15) noexcept;

      /**
       * @brief Set absolute accuracy.
       */
      void setAbsoluteAccuracy(double) noexcept;

      /**
       * @brief Get absolute accuracy.
       */
      double absoluteAccuracy() const noexcept;

      /**
       * @brief Attach AbsoluteAccuracy.
       *
       * When setAbsoluteAccuracy(double accuracy) is called, then also
       * other.setAbsoluteAccuracy(accuracy) is invoked.
       */
      void attachAbsoluteAccuracy(AbsoluteAccuracy* other);

      /**
       * @brief Detach verbosity before it gets deleted.
       */
      void detachAbsoluteAccuracy(AbsoluteAccuracy* other);

    private:
      void update(DesignPattern::Observer::Subject* changedSubject) final override;

      double absoluteAccuracy_ = 1e-15;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_ABSOLUTE_ACCURACY_HH
