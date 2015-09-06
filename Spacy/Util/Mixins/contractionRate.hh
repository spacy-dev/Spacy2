#ifndef SPACY_UTIL_MIXIN_CONTRACTION_RATE_HH
#define SPACY_UTIL_MIXIN_CONTRACTION_RATE_HH

#include "Spacy/Spaces/RealSpace/real.hh"

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
      explicit ContractionRate(Real desiredContraction = 0.25,
                               Real relaxedDesiredContraction = 0.5,
                               Real maximalContraction = 0.75) noexcept;

      /**
       * @brief Set contraction rate.
       * @param contraction monitored contraction rate
       */
      void setContraction(Real contraction) noexcept;

      /**
       * @brief Set desired contraction rate.
       * @param desiredContraction desired contraction rate
       */
      void setDesiredContraction(Real desiredContraction) noexcept;

      /**
       * @brief Set relaxed desired contraction rate.
       * @param relaxedDesiredContraction relaxed contraction rate
       */
      void setRelaxedDesiredContraction(Real relaxedDesiredContraction) noexcept;

      /**
       * @brief Set maximal contraction rate.
       * @param maximalContraction maximal allowed contraction rate
       */
      void setMaximalContraction(Real maximalContraction) noexcept;

      /**
       * @brief Access contraction rate.
       * @return monitored contraction rate
       */
      Real contraction() const noexcept;

      /**
       * @brief Access desired contraction rate.
       * @return desired contraction rate
       */
      Real desiredContraction() const noexcept;

      /**
       * @brief Get relaxed desired contraction rate.
       * @return relaxed contraction rate
       */
      Real relaxedDesiredContraction() const noexcept;

      /**
       * @brief Get desired contraction rate.
       * @return maximal allowed contraction rate
       */
      Real maximalContraction() const noexcept;

      /**
       * @brief Check if contraction is admissible.
       * @return contraction() < maximalContraction()
       */
      bool admissibleContraction() const noexcept;

    private:
      Real contraction_ = 1;
      Real desiredContraction_;
      Real relaxedDesiredContraction_;
      Real maximalContraction_;
    };
  }
}

#endif // SPACY_UTIL_MIXIN_CONTRACTION_RATE_HH
