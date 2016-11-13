#pragma once

#include <Spacy/Spaces/RealSpace/real.hh>

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
      explicit ContractionRate(Real desiredContraction = 0.25,
                               Real relaxedDesiredContraction = 0.5,
                               Real maximalContraction = 0.75) noexcept;

      void setContraction(Real contraction) noexcept;

      void setDesiredContraction(Real desiredContraction) noexcept;

      void setRelaxedDesiredContraction(Real relaxedDesiredContraction) noexcept;

      void setMaximalContraction(Real maximalContraction) noexcept;

      Real getContraction() const noexcept;

      Real getDesiredContraction() const noexcept;

      Real getRelaxedDesiredContraction() const noexcept;

      Real getMaximalContraction() const noexcept;

      bool contractionIsAdmissible() const noexcept;

    private:
      Real contraction_ = 1;
      Real desiredContraction_;
      Real relaxedDesiredContraction_;
      Real maximalContraction_;
    };
  }
}
