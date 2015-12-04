// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_UTIL_MIXIN_STATE_INDEX_HH
#define SPACY_UTIL_MIXIN_STATE_INDEX_HH

#include "mixinConnection.hh"

namespace Spacy
{
  namespace Mixin
  {
    /// %Mixin class for index of state variable.
    class StateIndex : public MixinConnection<StateIndex>
    {
    public:
      explicit StateIndex(unsigned index = 0) noexcept;

      void setStateIndex(unsigned index);

      double stateIndex() const noexcept;

      /// update function for observer pattern.
      void update(StateIndex* changedSubject);

    private:
      unsigned index_;
    };
  }
}

#endif // SPACY_UTIL_MIXIN_STATE_INDEX_HH
