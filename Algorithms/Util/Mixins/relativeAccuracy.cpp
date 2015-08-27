#include "relativeAccuracy.hh"

namespace Algorithm
{
  namespace Mixin
  {
    RelativeAccuracy::RelativeAccuracy(double relativeAccuracy) noexcept
      : relativeAccuracy_(relativeAccuracy)
    {}

    void RelativeAccuracy::setRelativeAccuracy(double relativeAccuracy)
    {
      relativeAccuracy_ = relativeAccuracy;
      notify();
    }

    double RelativeAccuracy::relativeAccuracy() const noexcept
    {
      return relativeAccuracy_;
    }

    void RelativeAccuracy::update(RelativeAccuracy* changedSubject)
    {
      setRelativeAccuracy( changedSubject->relativeAccuracy() );
    }

    void RelativeAccuracy::attachRelativeAccuracy(RelativeAccuracy& other)
    {
      attach(other);
    }

    void RelativeAccuracy::detachRelativeAccuracy(RelativeAccuracy& other)
    {
      detach(other);
    }
  }
}
