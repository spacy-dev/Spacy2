#include "absoluteAccuracy.hh"

namespace Spacy
{
  namespace Mixin
  {
    AbsoluteAccuracy::AbsoluteAccuracy(double accuracy) noexcept
      : absoluteAccuracy_(accuracy)
    {}

    void AbsoluteAccuracy::setAbsoluteAccuracy(double absoluteAccuracy)
    {
      absoluteAccuracy_ = absoluteAccuracy;
      notify();
    }

    double AbsoluteAccuracy::absoluteAccuracy() const noexcept
    {
      return absoluteAccuracy_;
    }

    void AbsoluteAccuracy::update(AbsoluteAccuracy* changedSubject)
    {
      setAbsoluteAccuracy( changedSubject->absoluteAccuracy() );
    }

    void AbsoluteAccuracy::attachAbsoluteAccuracy(AbsoluteAccuracy& other)
    {
      attach(other);
    }

    void AbsoluteAccuracy::detachAbsoluteAccuracy(AbsoluteAccuracy& other)
    {
      detach(other);
    }
  }
}
