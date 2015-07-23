#ifndef ALGORITHM_MIXIN_ITERATIVE_REFINEMENTS_HH
#define ALGORITHM_MIXIN_ITERATIVE_REFINEMENTS_HH

namespace Algorithm
{
  namespace Mixin
  {
    class IterativeRefinements
    {
    public:
      explicit IterativeRefinements(unsigned iterativeRefinements = 0);

      void setIterativeRefinements(unsigned iterativeRefinements);

      unsigned iterativeRefinements() const;

    private:
      unsigned iterativeRefinements_ = 0;
    };

  }
}

#endif // ALGORITHM_MIXIN_ITERATIVE_REFINEMENTS_HH
