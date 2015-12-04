// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_UTIL_MIXIN_ABSOLUTE_ACCURACY_HH
#define SPACY_UTIL_MIXIN_ABSOLUTE_ACCURACY_HH

#include "mixinConnection.hh"

namespace Spacy
{
  /// @cond
  class Real;
  /// @endcond

  namespace Mixin
  {
    /// %Mixin class for absolute accuracy.
    class AbsoluteAccuracy :public MixinConnection<AbsoluteAccuracy>
    {
    public:
      explicit AbsoluteAccuracy(double accuracy = 1e-15) noexcept;

      void setAbsoluteAccuracy(double accuracy);

      void setAbsoluteAccuracy(Real accuracy);

      double absoluteAccuracy() const noexcept;

      /// update function for observer pattern.
      void update(AbsoluteAccuracy* changedSubject);

    private:
      double absoluteAccuracy_;
    };
  }
}

#endif // SPACY_UTIL_MIXIN_ABSOLUTE_ACCURACY_HH
