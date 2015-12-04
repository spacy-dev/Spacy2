// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_UTIL_MIXIN_CONTROL_INDEX_HH
#define SPACY_UTIL_MIXIN_CONTROL_INDEX_HH

#include "mixinConnection.hh"

namespace Spacy
{
  namespace Mixin
  {
    /// %Mixin class for index of the control variable.
    class ControlIndex : public MixinConnection<ControlIndex>
    {
    public:
      explicit ControlIndex(unsigned index = 1) noexcept;

      void setControlIndex(unsigned index);

      double controlIndex() const noexcept;

      /// update function for observer pattern.
      void update(ControlIndex* changedSubject);

    private:
      unsigned index_;
    };
  }
}

#endif // SPACY_UTIL_MIXIN_CONTROL_INDEX_HH
