#ifndef SPACY_UTIL_MIXIN_DAMPING_ACCURACY_HH
#define SPACY_UTIL_MIXIN_DAMPING_ACCURACY_HH

namespace Spacy
{
  namespace Mixin
  {
    /**
     * @ingroup MixinGroup
     * @brief %Mixin class for the accuracy of damping factors.
     */
    class DampingAccuracy
    {
    public:
      /**
       * @brief Constructor.
       * @param accuracy accuracy for damping factors
       */
      explicit DampingAccuracy(double accuracy = 1e-3) noexcept;

      /**
       * @brief Set accuracy for damping factors.
       * @param accuracy accuracy for damping factors
       */
      void setDampingAccuracy(double accuracy) noexcept;

      /**
       * @brief Access accuracy for damping factors.
       * @return accuracy for damping factors
       */
      double dampingAccuracy() const noexcept;

    private:
      double dampingAccuracy_;
    };
  }
}

#endif // SPACY_UTIL_MIXIN_DAMPING_ACCURACY_HH
