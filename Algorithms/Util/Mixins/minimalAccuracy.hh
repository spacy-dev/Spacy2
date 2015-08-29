#ifndef ALGORITHM_UTIL_MIXIN_MINIMAL_ACCURACY_HH
#define ALGORITHM_UTIL_MIXIN_MINIMAL_ACCURACY_HH

#include "mixinConnection.hh"

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @ingroup MixinGroup
     * @brief Mixin class for minimal accuracy.
     */
    class MinimalAccuracy : public MixinConnection<MinimalAccuracy>
    {
    public:
      /**
       * @brief Constructor. Sets minimal accuracy.
       */
      explicit MinimalAccuracy(double minimalAccuracy = 0.25) noexcept;

      /**
       * @brief Set minimal accuracy.
       */
      void setMinimalAccuracy(double);

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
      void attachMinimalAccuracy(MinimalAccuracy& other);

      /**
       * @brief Detach RelativeAccuracy before it gets deleted.
       */
      void detachMinimalAccuracy(MinimalAccuracy& other);

      /**
       * @brief update function for observer pattern.
       */
      void update(MinimalAccuracy* changedSubject);

    private:
      double minimalAccuracy_;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_MINIMAL_ACCURACY_HH
