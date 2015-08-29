#ifndef ALGORITHM_UTIL_MIXIN_CONTRACTION_RATE_HH
#define ALGORITHM_UTIL_MIXIN_CONTRACTION_RATE_HH

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @ingroup MixinGroup
     * @brief Mixin class for contraction rates.
     */
    class ContractionRate
    {
    public:
      /**
       * @brief Constructor. Sets desired and maximal contraction rate.
       */
      explicit ContractionRate(double desiredContraction = 0.25, double relaxedDesiredContraction = 0.5, double maximalContraction = 0.75) noexcept;

      /**
       * @brief Set contraction rate.
       */
      void setContraction(double contraction) noexcept;

      /**
       * @brief Set desired contraction rate.
       */
      void setDesiredContraction(double desiredContraction) noexcept;

      /**
       * @brief Set relaxed desired contraction rate.
       */
      void setRelaxedDesiredContraction(double relaxedDesiredContraction) noexcept;

      /**
       * @brief Set maximal contraction rate.
       */
      void setMaximalContraction(double maximalContraction) noexcept;

      /**
       * @brief Get contraction rate.
       */
      double contraction() const noexcept;

      /**
       * @brief Get desired contraction rate.
       */
      double desiredContraction() const noexcept;

      /**
       * @brief Get relaxed desired contraction rate.
       */
      double relaxedDesiredContraction() const noexcept;

      /**
       * @brief Get desired contraction rate.
       */
      double maximalContraction() const noexcept;

      /**
       * @brief Check if contraction is admissible.
       * @return contraction() < maximalContraction()
       */
      bool admissibleContraction() const noexcept;

    private:
      double contraction_ = 1;
      double desiredContraction_;
      double relaxedDesiredContraction_;
      double maximalContraction_;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_CONTRACTION_RATE_HH
