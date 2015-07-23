#include "maxSteps.hh"

namespace Algorithm
{
  namespace Mixin
  {
    MaxSteps::MaxSteps(unsigned maxSteps) noexcept
      : maxSteps_(maxSteps)
    {}

    void MaxSteps::setMaxSteps(unsigned maxSteps) noexcept
    {
      maxSteps_ = maxSteps;
    }

    unsigned MaxSteps::maxSteps() const noexcept
    {
      return maxSteps_;
    }
  }
}
