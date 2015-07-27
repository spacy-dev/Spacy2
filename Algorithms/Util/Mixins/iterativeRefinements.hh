#ifndef ALGORITHM_MIXIN_ITERATIVE_REFINEMENTS_HH
#define ALGORITHM_MIXIN_ITERATIVE_REFINEMENTS_HH

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @brief Mixin class for iterative refinements.
     */
    class IterativeRefinements
    {
    public:
      /**
       * @brief Constructor.
       * @param Sets number of iterative refinements.
       */
      explicit IterativeRefinements(unsigned iterativeRefinements = 0);

      /**
       * @brief Set number of iterative refinements.
       */
      void setIterativeRefinements(unsigned iterativeRefinements);

      /**
       * @brief Get number of iterative refinements.
       */
      unsigned iterativeRefinements() const;

    private:
      unsigned iterativeRefinements_ = 0;
    };

  }
}

#endif // ALGORITHM_MIXIN_ITERATIVE_REFINEMENTS_HH
