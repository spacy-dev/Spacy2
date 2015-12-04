// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_UTIL_MIXIN_RELATIVE_ACCURACY_HH
#define SPACY_UTIL_MIXIN_RELATIVE_ACCURACY_HH

#include "mixinConnection.hh"

namespace Spacy
{
  namespace Mixin
  {
    /// %Mixin class for relative accuracy.
    class RelativeAccuracy : public MixinConnection<RelativeAccuracy>
    {
    public:
      explicit RelativeAccuracy(double accuracy = 1e-15) noexcept;

      void setRelativeAccuracy(double accuracy);

      double relativeAccuracy() const noexcept;

      /// update function for observer pattern.
      void update(RelativeAccuracy* changedSubject);

    private:
      double relativeAccuracy_;
    };
  }
}

#endif // SPACY_UTIL_MIXIN_RELATIVE_ACCURACY_HH
