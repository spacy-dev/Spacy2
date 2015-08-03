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

    void MinimalAccuracy::update(DesignPattern::Observer::Subject *changedSubject)
    {
      if( changedSubject == this ) return;
      setMinimalAccuracy( dynamic_cast<MinimalAccuracy*>(changedSubject)->minimalAccuracy() );
    }

    void MinimalAccuracy::attachMinimalAccuracy(MinimalAccuracy* other)
    {
      attach(other);
    }

    void MinimalAccuracy::detachMinimalAccuracy(MinimalAccuracy* other)
    {
      detach(other);
    }
  }
}
