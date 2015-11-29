// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "numberOfThreads.hh"

namespace Spacy
{
  namespace Mixin
  {
    void NumberOfThreads::setNumberOfThreads(unsigned nThreads)
    {
      nThreads_ = nThreads;
    }

    unsigned NumberOfThreads::nThreads() const noexcept
    {
      return nThreads_;
    }
  }
}
