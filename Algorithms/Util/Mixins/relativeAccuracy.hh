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
       * @brief Constructor.
       * @param accuracy relative accuracy.
       */
      explicit RelativeAccuracy(double accuracy = 1e-15) noexcept;

      /**
       * @brief Set relative accuracy.
       * @param accuracy relative accuracy
       */
      void setRelativeAccuracy(double accuracy);

      /**
       * @brief Access relative accuracy.
       * @return relative accuracy
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

      /**
       * @brief update function for observer pattern.
       */
      void update(RelativeAccuracy* changedSubject);

    private:
      double relativeAccuracy_;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_RELATIVE_ACCURACY_HH
