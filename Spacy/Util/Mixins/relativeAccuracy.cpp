// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "relativeAccuracy.hh"

namespace Spacy
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
  }
}
