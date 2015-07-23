#ifndef ALGORITHM_UTIL_MIXIN_CONTRACTION_RATE_HH
#define ALGORITHM_UTIL_MIXIN_CONTRACTION_RATE_HH

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @brief Mixin class for contraction rates.
     */
    class ContractionRate
    {
    public:
      /**
       * @brief Constructor. Sets desired and maximal contraction rate.
       */
      explicit ContractionRate(double desiredContraction = 0.5, double maximalContraction = 0.75) noexcept;

      /**
       * @brief Set desired contraction rate.
       */
      void setDesiredContraction(double desiredContraction) noexcept;

      /**
       * @brief Set maximal contraction rate.
       */
      void setMaximalContraction(double maximalContraction) noexcept;

      /**
       * @brief Get desired contraction rate.
       */
      double desiredContraction() const noexcept;

      /**
       * @brief Get desired contraction rate.
       */
      double maximalContraction() const noexcept;

      /**
       * @brief Check if contraction is admissible.
       * @return contraction < maximalContraction()
       */
      bool admissibleContraction(double contraction) const noexcept;

    private:
      double desiredContraction_ = 0.5;
      double maximalContraction_ = 0.75;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_CONTRACTION_RATE_HH
