#ifndef ALGORITHM_UTIL_MIXIN_RELATIVE_ACCURACY_HH
#define ALGORITHM_UTIL_MIXIN_RELATIVE_ACCURACY_HH

#include "mixinConnection.hh"

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @ingroup MixinGroup
     * @brief Mixin class for relative accuracy.
     */
    class RelativeAccuracy : public MixinConnection<RelativeAccuracy>
    {
    public:
      /**
       * @brief Constructor. Sets relative accuracy.
       */
      explicit RelativeAccuracy(double relativeAccuracy = 1e-15) noexcept;

      /**
       * @brief Set relative accuracy.
       */
      void setRelativeAccuracy(double) noexcept;

      /**
       * @brief Get relative accuracy.
       */
      double relativeAccuracy() const noexcept;

      /**
       * @brief Attach RelativeAccuracy.
       *
       * When setRelativeAccuracy(double relativeAccuracy) is called, then also
       * other.setRelativeAccuracy(relativeAccuracy) is invoked.
       */
      void attachRelativeAccuracy(RelativeAccuracy& other);

      /**
       * @brief Detach RelativeAccuracy before it gets deleted.
       */
      void detachRelativeAccuracy(RelativeAccuracy& other);

      void update(RelativeAccuracy* changedSubject);

    private:
      double relativeAccuracy_ = 1e-15;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_RELATIVE_ACCURACY_HH
