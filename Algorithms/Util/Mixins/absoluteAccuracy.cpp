#include "absoluteAccuracy.hh"

namespace Algorithm
{
  namespace Mixin
  {
    AbsoluteAccuracy::AbsoluteAccuracy(double absoluteAccuracy)
      : absoluteAccuracy_(absoluteAccuracy)
    {}

    void AbsoluteAccuracy::setAbsoluteAccuracy(double absoluteAccuracy) noexcept
    {
      absoluteAccuracy_ = absoluteAccuracy;
    }

    double AbsoluteAccuracy::absoluteAccuracy() const noexcept
    {
      return absoluteAccuracy_;
    }
  }
}
