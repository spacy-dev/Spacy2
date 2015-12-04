// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "iterativeRefinements.hh"

namespace Spacy
{
  namespace Mixin
  {
    IterativeRefinements::IterativeRefinements(unsigned refinements)
      : iterativeRefinements_(refinements)
    {}

    void IterativeRefinements::setIterativeRefinements(unsigned refinements)
    {
      iterativeRefinements_ = refinements;
      notify();
    }

    unsigned IterativeRefinements::iterativeRefinements() const
    {
      return iterativeRefinements_;
    }

    void IterativeRefinements::update(IterativeRefinements* changedSubject)
    {
      setIterativeRefinements( changedSubject->iterativeRefinements() );
    }
  }
}
