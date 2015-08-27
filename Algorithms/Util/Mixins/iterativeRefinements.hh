#ifndef ALGORITHM_MIXIN_ITERATIVE_REFINEMENTS_HH
#define ALGORITHM_MIXIN_ITERATIVE_REFINEMENTS_HH

#include "mixinConnection.hh"

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @ingroup MixinGroup
     * @brief Mixin class for iterative refinements.
     */
    class IterativeRefinements : public MixinConnection<IterativeRefinements>
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

      /**
       * @brief Attach IterativeRefinements.
       *
       * When setIterativeRefinements(unsigned iterativeRefinements) is called, then also
       * other.setIterativeRefinements(iterativeRefinements) is invoked.
       */
      void attachIterativeRefinements(IterativeRefinements& other);

      /**
       * @brief Detach Eps before it gets deleted.
       */
      void detachIterativeRefinements(IterativeRefinements& other);

      /**
       * @brief update function for observer pattern.
       */
      void update(IterativeRefinements* changedSubject) ;

    private:
      unsigned iterativeRefinements_ = 0;
    };

  }
}

#endif // ALGORITHM_MIXIN_ITERATIVE_REFINEMENTS_HH
