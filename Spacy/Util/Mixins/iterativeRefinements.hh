// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_MIXIN_ITERATIVE_REFINEMENTS_HH
#define SPACY_MIXIN_ITERATIVE_REFINEMENTS_HH

#include "mixinConnection.hh"

namespace Spacy
{
  namespace Mixin
  {
    /**
     * @ingroup MixinGroup
     * @brief %Mixin class for iterative refinements.
     */
    class IterativeRefinements : public MixinConnection<IterativeRefinements>
    {
    public:
      /**
       * @brief Constructor.
       * @param refinements number of iterative refinements.
       */
      explicit IterativeRefinements(unsigned refinements = 0);

      /**
       * @brief Set number of iterative refinements.
       * @param refinements number of iterative refinements
       */
      void setIterativeRefinements(unsigned refinements);

      /**
       * @brief Access number of iterative refinements.
       * @return number of iterative refinements
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
      unsigned iterativeRefinements_;
    };

  }
}

#endif // SPACY_MIXIN_ITERATIVE_REFINEMENTS_HH
