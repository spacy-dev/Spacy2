// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_UTIL_MIXIN_CONTRACTION_RATE_HH
#define SPACY_UTIL_MIXIN_CONTRACTION_RATE_HH

#include "Spacy/Spaces/RealSpace/real.hh"

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

      Real contraction() const noexcept;

      Real desiredContraction() const noexcept;

      Real relaxedDesiredContraction() const noexcept;

      Real maximalContraction() const noexcept;

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
