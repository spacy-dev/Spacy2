// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_CONJUGATE_GRADIENTS_TRIVIALPRECONDITIONER_HH
#define SPACY_CONJUGATE_GRADIENTS_TRIVIALPRECONDITIONER_HH

#include "Spacy/linearSolver.hh"
#include "Spacy/operator.hh"
#include "Spacy/vector.hh"
#include "Spacy/Util/Base/operatorBase.hh"

namespace Spacy
{
  /// \cond
  class VectorSpace;
  /// \endcond

  namespace CG
  {
    /**
     * @brief A trivial Preconditioner that does nothing, i.e., returns the argument
     *
     */
    class TrivialPreconditioner
        : public OperatorBase
    {
    public:
      /**
       * @brief Constructor.
       * @param domain domain space
       * @param range range space
       */
      TrivialPreconditioner( const VectorSpace& domain,
                             const VectorSpace& range);

      /**
       * @brief Apply preconditioner \f$P\f$.
       * @param x argument
       * @return \f$P(x)\f$
       */
      Vector operator()(const Vector& x) const;

      
    private:
        
    };
  }
}

#endif // SPACY_CONJUGATE_GRADIENTS_TRIVIALPRECONDITIONER_HH
