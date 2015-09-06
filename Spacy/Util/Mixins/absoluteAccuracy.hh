#ifndef SPACY_UTIL_MIXIN_ABSOLUTE_ACCURACY_HH
#define SPACY_UTIL_MIXIN_ABSOLUTE_ACCURACY_HH

#include "mixinConnection.hh"

namespace Spacy
{
  /// @cond
  class Real;
  /// @endcond

  namespace Mixin
  {
    /**
     * @ingroup MixinGroup
     * @brief %Mixin class for absolute accuracy.
     */
    class AbsoluteAccuracy :public MixinConnection<AbsoluteAccuracy>
    {
    public:
      /**
       * @brief Constructor.
       * @param accuracy absolute accuracy
       */
      explicit AbsoluteAccuracy(double accuracy = 1e-15) noexcept;

      /**
       * @brief Set absolute accuracy.
       * @param accuracy absolute accuracy
       */
      void setAbsoluteAccuracy(double accuracy);

      void setAbsoluteAccuracy(Real accuracy);

      /**
       * @brief Access absolute accuracy.
       * @return absolute accuracy
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
      double absoluteAccuracy_;
    };
  }
}

#endif // SPACY_UTIL_MIXIN_ABSOLUTE_ACCURACY_HH
