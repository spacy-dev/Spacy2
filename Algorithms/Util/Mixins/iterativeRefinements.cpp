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
    }

    unsigned IterativeRefinements::iterativeRefinements() const
    {
      return iterativeRefinements_;
    }
  }
}
