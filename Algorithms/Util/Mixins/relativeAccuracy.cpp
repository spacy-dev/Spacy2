#include "relativeAccuracy.hh"

namespace Algorithm
{
  namespace Mixin
  {
    RelativeAccuracy::RelativeAccuracy(double relativeAccuracy) noexcept
      : relativeAccuracy_(relativeAccuracy)
    {}

    void RelativeAccuracy::setRelativeAccuracy(double relativeAccuracy) noexcept
    {
      relativeAccuracy_ = relativeAccuracy;
      connection_.forward(relativeAccuracy_);
    }

    double RelativeAccuracy::relativeAccuracy() const noexcept
    {
      return relativeAccuracy_;
    }
  }
}
