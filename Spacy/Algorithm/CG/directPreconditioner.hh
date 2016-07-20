// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_CONJUGATE_GRADIENTS_DIRECTPRECONDITIONER_HH
#define SPACY_CONJUGATE_GRADIENTS_DIRECTPRECONDITIONER_HH

#include "Spacy/linearSolver.hh"
#include "Spacy/operator.hh"
#include "Spacy/c2Functional.hh"
#include "Spacy/vector.hh"
#include "Spacy/Util/Base/operatorBase.hh"

#include <iostream>

namespace Spacy
{
  /// \cond
  class VectorSpace;
  /// \endcond

  namespace CG
  {
    /**
     * @brief A Preconditioner that uses a direct solver
     *
     */
     
    class DirectPreconditioner
        : public OperatorBase
    {
    public:
      /**
       * @brief Constructor.
       * @param domain domain space
       * @param range range space
       */
      DirectPreconditioner( ::Spacy::C2Functional M,
                            const ::Spacy::Vector& origin,  
                            const VectorSpace& domain,
                            const VectorSpace& range);

      /**
       * @brief Apply preconditioner \f$P\f$.
       * @param x argument
       * @return \f$P(x)\f$
       */
      Vector operator()(const Vector& x) const;

      
    private:
        ::Spacy::C2Functional M_;
        const ::Spacy::Vector& origin_;
        std::shared_ptr<::Spacy::LinearSolver> directSolver;

    };
  }
}

#endif // SPACY_CONJUGATE_GRADIENTS_TRIVIALPRECONDITIONER_HH
