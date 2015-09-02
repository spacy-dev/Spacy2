#ifndef ALGORITHM_UTIL_MIXIN_CONTRACTION_RATE_HH
#define ALGORITHM_UTIL_MIXIN_CONTRACTION_RATE_HH

namespace Spacy
{
  namespace Mixin
  {
    /**
     * @ingroup MixinGroup
     * @brief %Mixin class for contraction rates.
     */
    class ContractionRate
    {
    public:
      /**
       * @brief Constructor.
       * @param desiredContraction desired contraction rate
       * @param relaxedDesiredContraction relaxed contraction rate
       * @param maximalContraction maximal allowed contraction rate
       */
      explicit ContractionRate(double desiredContraction = 0.25,
                               double relaxedDesiredContraction = 0.5,
                               double maximalContraction = 0.75) noexcept;

      /**
       * @brief Set contraction rate.
       * @param contraction monitored contraction rate
       */
      void setContraction(double contraction) noexcept;

      /**
       * @brief Set desired contraction rate.
       * @param desiredContraction desired contraction rate
       */
      void setDesiredContraction(double desiredContraction) noexcept;

      /**
       * @brief Set relaxed desired contraction rate.
       * @param relaxedDesiredContraction relaxed contraction rate
       */
      void setRelaxedDesiredContraction(double relaxedDesiredContraction) noexcept;

      /**
       * @brief Set maximal contraction rate.
       * @param maximalContraction maximal allowed contraction rate
       */
      void setMaximalContraction(double maximalContraction) noexcept;

      /**
       * @brief Access contraction rate.
       * @return monitored contraction rate
       */
      double contraction() const noexcept;

      /**
       * @brief Access desired contraction rate.
       * @return desired contraction rate
       */
      double desiredContraction() const noexcept;

      /**
       * @brief Get relaxed desired contraction rate.
       * @return relaxed contraction rate
       */
      double relaxedDesiredContraction() const noexcept;

      /**
       * @brief Get desired contraction rate.
       * @return maximal allowed contraction rate
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
