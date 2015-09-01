#include "relativeAccuracy.hh"

namespace VSA
{
  namespace Mixin
  {
    RelativeAccuracy::RelativeAccuracy(double accuracy) noexcept
      : relativeAccuracy_(accuracy)
    {}

    void RelativeAccuracy::setRelativeAccuracy(double accuracy)
    {
      relativeAccuracy_ = accuracy;
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
