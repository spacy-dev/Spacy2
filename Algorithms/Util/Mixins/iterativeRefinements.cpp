#include "iterativeRefinements.hh"

namespace Algorithm
{
  namespace Mixin
  {
    IterativeRefinements::IterativeRefinements(unsigned iterativeRefinements)
      : iterativeRefinements_(iterativeRefinements)
    {}

    void IterativeRefinements::setIterativeRefinements(unsigned iterativeRefinements)
    {
      iterativeRefinements_ = iterativeRefinements;
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
