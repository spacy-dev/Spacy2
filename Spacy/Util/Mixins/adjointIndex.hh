// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_UTIL_MIXIN_ADJOINT_INDEX_HH
#define SPACY_UTIL_MIXIN_ADJOINT_INDEX_HH

#include "mixinConnection.hh"

namespace Spacy
{
  namespace Mixin
  {
    /// %Mixin class for index of the adjoint variable.
    class AdjointIndex : public MixinConnection<AdjointIndex>
    {
    public:
      explicit AdjointIndex(unsigned index = 2) noexcept;

      void setAdjointIndex(unsigned index);

      double adjointIndex() const noexcept;

      /// f update function for observer pattern.
      void update(AdjointIndex* changedSubject);

    private:
      unsigned index_;
    };
  }
}

#endif // SPACY_UTIL_MIXIN_ADJOINT_INDEX_HH
