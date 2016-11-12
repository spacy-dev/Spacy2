#pragma once

namespace Spacy
{
  namespace Mixin
  {
    /// %Mixin class for contraction rates.
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

      void setContraction(double contraction) noexcept;

      void setDesiredContraction(double desiredContraction) noexcept;

      void setRelaxedDesiredContraction(double relaxedDesiredContraction) noexcept;

      void setMaximalContraction(double maximalContraction) noexcept;

      double getContraction() const noexcept;

      double getDesiredContraction() const noexcept;

      double getRelaxedDesiredContraction() const noexcept;

      double getMaximalContraction() const noexcept;

      bool contractionIsAdmissible() const noexcept;

    private:
      double contraction_ = 1;
      double desiredContraction_;
      double relaxedDesiredContraction_;
      double maximalContraction_;
    };
  }
}
