// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_ADAPTER_EIGEN_LINEAR_OPERATOR_CREATOR_HH
#define SPACY_ADAPTER_EIGEN_LINEAR_OPERATOR_CREATOR_HH

#include "Spacy/Adapter/Generic/linearOperatorCreator.hh"
#include "linearOperator.hh"

namespace Spacy
{
  namespace Rn
  {
    /// %VectorCreator for linear operators \f$X\rightarrow Y\f$, based on the %Eigen library.
    struct LinearOperatorCreator : Generic::LinearOperatorCreator< LinearOperator >
    {
      LinearOperatorCreator(const VectorSpace& X, const VectorSpace& Y);
    };
  }
}

#endif // SPACY_ADAPTER_EIGEN_LINEAR_OPERATOR_CREATOR_HH
