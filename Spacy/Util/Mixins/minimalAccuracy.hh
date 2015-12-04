// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_UTIL_MIXIN_MINIMAL_ACCURACY_HH
#define SPACY_UTIL_MIXIN_MINIMAL_ACCURACY_HH

#include "mixinConnection.hh"

namespace Spacy
{
  namespace Mixin
  {
    /// %Mixin class for minimal accuracy.
    class MinimalAccuracy : public MixinConnection<MinimalAccuracy>
    {
    public:
      explicit MinimalAccuracy(double accuracy = 0.25) noexcept;

      void setMinimalAccuracy(double accuracy);

      double minimalAccuracy() const noexcept;

      /// update function for observer pattern.
      void update(MinimalAccuracy* changedSubject);

    private:
      double minimalAccuracy_;
    };
  }
}

#endif // SPACY_UTIL_MIXIN_MINIMAL_ACCURACY_HH
