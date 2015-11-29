// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "absoluteAccuracy.hh"

#include "Spacy/Spaces/RealSpace/real.hh"

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

    void AbsoluteAccuracy::setAbsoluteAccuracy(Real accuracy)
    {
      absoluteAccuracy_ = toDouble(accuracy);
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
