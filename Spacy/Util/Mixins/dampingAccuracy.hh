// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_UTIL_MIXIN_DAMPING_ACCURACY_HH
#define SPACY_UTIL_MIXIN_DAMPING_ACCURACY_HH

namespace Spacy
{
  namespace Mixin
  {
    /// %Mixin class for the accuracy of damping factors.
    class DampingAccuracy
    {
    public:
      explicit DampingAccuracy(double accuracy = 1e-3) noexcept;

      void setDampingAccuracy(double accuracy) noexcept;

      double dampingAccuracy() const noexcept;

    private:
      double dampingAccuracy_;
    };
  }
}

#endif // SPACY_UTIL_MIXIN_DAMPING_ACCURACY_HH
