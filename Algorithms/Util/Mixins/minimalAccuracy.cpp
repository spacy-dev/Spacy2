#include "minimalAccuracy.hh"

namespace Algorithm
{
  namespace Mixin
  {
    MinimalAccuracy::MinimalAccuracy(double minimalAccuracy) noexcept
      : minimalAccuracy_(minimalAccuracy)
    {}

    void MinimalAccuracy::setMinimalAccuracy(double minimalAccuracy) noexcept
    {
      minimalAccuracy_ = minimalAccuracy;
    }

    double MinimalAccuracy::minimalAccuracy() const noexcept
    {
      return minimalAccuracy_;
    }
  }
}
