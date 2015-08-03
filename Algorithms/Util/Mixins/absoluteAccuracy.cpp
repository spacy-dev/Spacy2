#include "absoluteAccuracy.hh"

namespace Algorithm
{
  namespace Mixin
  {
    AbsoluteAccuracy::AbsoluteAccuracy(double absoluteAccuracy) noexcept
      : absoluteAccuracy_(absoluteAccuracy)
    {}

    void AbsoluteAccuracy::setAbsoluteAccuracy(double absoluteAccuracy) noexcept
    {
      absoluteAccuracy_ = absoluteAccuracy;
      notify();
    }

    double AbsoluteAccuracy::absoluteAccuracy() const noexcept
    {
      return absoluteAccuracy_;
    }

    void AbsoluteAccuracy::update(DesignPattern::Observer::Subject *changedSubject)
    {
      if( changedSubject == this ) return;
      setAbsoluteAccuracy( dynamic_cast<AbsoluteAccuracy*>(changedSubject)->absoluteAccuracy() );
    }

    void AbsoluteAccuracy::attachAbsoluteAccuracy(AbsoluteAccuracy* other)
    {
      attach(other);
    }

    void AbsoluteAccuracy::detachAbsoluteAccuracy(AbsoluteAccuracy* other)
    {
      detach(other);
    }
  }
}
