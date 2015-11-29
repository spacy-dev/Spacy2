// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "minimalAccuracy.hh"

namespace Spacy
{
  namespace Mixin
  {
    MinimalAccuracy::MinimalAccuracy(double accuracy) noexcept
      : minimalAccuracy_(accuracy)
    {}

    void MinimalAccuracy::setMinimalAccuracy(double accuracy)
    {
      minimalAccuracy_ = accuracy;
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
