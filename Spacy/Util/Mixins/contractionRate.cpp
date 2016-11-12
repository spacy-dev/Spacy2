#include "contractionRate.hh"

#include <cassert>

namespace Spacy
{
  namespace Mixin
  {
    ContractionRate::ContractionRate(double desiredContraction,
                                     double relaxedDesiredContraction,
                                     double maximalContraction) noexcept
      : desiredContraction_(desiredContraction),
        relaxedDesiredContraction_(relaxedDesiredContraction),
        maximalContraction_(maximalContraction)
    {
      assert( desiredContraction_ < relaxedDesiredContraction_ );
      assert( relaxedDesiredContraction_ < maximalContraction_ );
    }

    void ContractionRate::setContraction(double contraction) noexcept
    {
      contraction_ = contraction;
    }

    void ContractionRate::setDesiredContraction(double desiredContraction) noexcept
    {
      desiredContraction_ = desiredContraction;
    }

    void ContractionRate::setRelaxedDesiredContraction(double relaxedDesiredContraction) noexcept
    {
      relaxedDesiredContraction_ = relaxedDesiredContraction;
    }

    void ContractionRate::setMaximalContraction(double maximalContraction) noexcept
    {
      maximalContraction_ = maximalContraction;
    }

    double ContractionRate::getContraction() const noexcept
    {
      return contraction_;
    }

    double ContractionRate::getDesiredContraction() const noexcept
    {
      return desiredContraction_;
    }

    double ContractionRate::getRelaxedDesiredContraction() const noexcept
    {
      return relaxedDesiredContraction_;
    }

    double ContractionRate::getMaximalContraction() const noexcept
    {
      return maximalContraction_;
    }

    bool ContractionRate::contractionIsAdmissible() const noexcept
    {
      return getContraction() < getMaximalContraction();
    }
  }
}
