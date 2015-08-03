#ifndef ALGORITHM_UTIL_MIXIN_MINIMAL_ACCURACY_HH
#define ALGORITHM_UTIL_MIXIN_MINIMAL_ACCURACY_HH

#include "Util/DesignPatterns/observer.hh"

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @brief Mixin class for minimal accuracy.
     */
    class MinimalAccuracy : public DesignPattern::Observer::Subject , public DesignPattern::Observer::Observer
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

      /**
       * @brief Attach MinimalAccuracy.
       *
       * When setMinimalAccuracy(double minimalAccuracy) is called, then also
       * other.setMinimalAccuracy(minimalAccuracy) is invoked.
       */
      void attachMinimalAccuracy(MinimalAccuracy* other);

      /**
       * @brief Detach RelativeAccuracy before it gets deleted.
       */
      void detachMinimalAccuracy(MinimalAccuracy* other);

    private:
      void update(DesignPattern::Observer::Subject* changedSubject) final override;

      double minimalAccuracy_ = 0.25;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_MINIMAL_ACCURACY_HH
