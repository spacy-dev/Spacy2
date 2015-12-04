// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "maxSteps.hh"

namespace Spacy
{
  namespace Mixin
  {
    MaxSteps::MaxSteps(unsigned maxSteps) noexcept
      : maxSteps_(maxSteps)
    {}

    void MaxSteps::setMaxSteps(unsigned maxSteps)
    {
      maxSteps_ = maxSteps;
      notify();
    }

    unsigned MaxSteps::maxSteps() const noexcept
    {
      return maxSteps_;
    }


    void MaxSteps::update(MaxSteps* changedSubject)
    {
      setMaxSteps( changedSubject->maxSteps() );
    }
  }
}
