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

    void IterativeRefinements::attachIterativeRefinements(IterativeRefinements& other)
    {
      attach(other);
    }

    void IterativeRefinements::detachIterativeRefinements(IterativeRefinements& other)
    {
      detach(other);
    }
  }
}
