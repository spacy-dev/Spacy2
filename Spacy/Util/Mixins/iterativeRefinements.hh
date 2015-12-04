// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_MIXIN_ITERATIVE_REFINEMENTS_HH
#define SPACY_MIXIN_ITERATIVE_REFINEMENTS_HH

#include "mixinConnection.hh"

namespace Spacy
{
  namespace Mixin
  {
    /// %Mixin class for iterative refinements.
    class IterativeRefinements : public MixinConnection<IterativeRefinements>
    {
    public:
      explicit IterativeRefinements(unsigned refinements = 0);

      void setIterativeRefinements(unsigned refinements);

      unsigned iterativeRefinements() const;

      /// update function for observer pattern.
      void update(IterativeRefinements* changedSubject) ;

    private:
      unsigned iterativeRefinements_;
    };

  }
}

#endif // SPACY_MIXIN_ITERATIVE_REFINEMENTS_HH
