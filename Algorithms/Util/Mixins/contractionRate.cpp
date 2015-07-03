#include "contractionRate.hh"

namespace Algorithm
{
  namespace Mixin
  {
    ContractionRate::ContractionRate(double desiredContraction, double maximalContraction)
      : desiredContraction_(desiredContraction), maximalContraction_(maximalContraction)
    {}

    void ContractionRate::setDesiredContraction(double desiredContraction) noexcept
    {
      desiredContraction_ = desiredContraction;
    }

    void ContractionRate::setMaximalContraction(double maximalContraction) noexcept
    {
      maximalContraction_ = maximalContraction;
    }

    double ContractionRate::desiredContraction() const noexcept
    {
      return desiredContraction_;
    }

    double ContractionRate::maximalContraction() const noexcept
    {
      return maximalContraction_;
    }

    bool ContractionRate::admissibleContraction(double contraction) const noexcept
    {
      return contraction < maximalContraction();
    }
  }
}
