// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "dampingAccuracy.hh"

namespace Spacy
{
  namespace Mixin
  {
    DampingAccuracy::DampingAccuracy(double accuracy) noexcept
      : dampingAccuracy_(accuracy)
    {}

    void DampingAccuracy::setDampingAccuracy(double accuracy) noexcept
    {
      dampingAccuracy_ = accuracy;
    }

    double DampingAccuracy::dampingAccuracy() const noexcept
    {
      return dampingAccuracy_;
    }
  }
}
