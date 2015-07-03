#include "relativeAccuracy.hh"

namespace Algorithm
{
  namespace Mixin
  {
    RelativeAccuracy::RelativeAccuracy(double relativeAccuracy)
      : relativeAccuracy_(relativeAccuracy)
    {}

    void RelativeAccuracy::setRelativeAccuracy(double relativeAccuracy) noexcept
    {
      relativeAccuracy_ = relativeAccuracy;
    }

    double RelativeAccuracy::relativeAccuracy() const noexcept
    {
      return relativeAccuracy_;
    }
  }
}
