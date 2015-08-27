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
      notify();
    }

    double MinimalAccuracy::minimalAccuracy() const noexcept
    {
      return minimalAccuracy_;
    }

    void MinimalAccuracy::update(MinimalAccuracy* changedSubject)
    {
      setMinimalAccuracy( changedSubject->minimalAccuracy() );
    }

    void MinimalAccuracy::attachMinimalAccuracy(MinimalAccuracy& other)
    {
      attach(other);
    }

    void MinimalAccuracy::detachMinimalAccuracy(MinimalAccuracy& other)
    {
      detach(other);
    }
  }
}
