#ifndef ALGORITHM_UTIL_MIXIN_ABSOLUTE_ACCURACY_HH
#define ALGORITHM_UTIL_MIXIN_ABSOLUTE_ACCURACY_HH

#include "mixinConnection.hh"

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @ingroup MixinGroup
     * @brief Mixin class for absolute accuracy.
     */
    class AbsoluteAccuracy :public MixinConnection<AbsoluteAccuracy>
    {
    public:
      /**
       * @brief Constructor. Sets absolute accuracy.
       */
      explicit AbsoluteAccuracy(double absoluteAccuracy = 1e-15) noexcept;

      /**
       * @brief Set absolute accuracy.
       */
      void setAbsoluteAccuracy(double);

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
      void attachAbsoluteAccuracy(AbsoluteAccuracy& other);

      /**
       * @brief Detach verbosity before it gets deleted.
       */
      void detachAbsoluteAccuracy(AbsoluteAccuracy& other);

      /**
       * @brief update function for observer pattern.
       */
      void update(AbsoluteAccuracy* changedSubject);

    private:
      double absoluteAccuracy_ = 1e-15;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_ABSOLUTE_ACCURACY_HH
