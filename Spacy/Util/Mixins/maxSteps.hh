// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_UTIL_MIXIN_MAXSTEPS_HH
#define SPACY_UTIL_MIXIN_MAXSTEPS_HH

#include "mixinConnection.hh"

namespace Spacy
{
  namespace Mixin
  {
    /// %Mixin class for maximal number of steps/iterations.
    class MaxSteps : public MixinConnection<MaxSteps>
    {
    public:
      explicit MaxSteps(unsigned maxSteps = 100) noexcept;

      void setMaxSteps(unsigned maxSteps);

      unsigned maxSteps() const noexcept;

      /// update function for observer pattern.
      void update(MaxSteps* changedSubject);

    private:
      unsigned maxSteps_;
    };
  }
}

#endif // SPACY_UTIL_MIXIN_MAXSTEPS_HH
