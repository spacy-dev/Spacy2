// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_ADAPTER_EIGEN_VECTOR_HH
#define SPACY_ADAPTER_EIGEN_VECTOR_HH

#include "Eigen/Dense"

#include "Spacy/Adapter/Generic/vector.hh"

namespace Spacy
{
  namespace Rn
  {
    /**
     * @ingroup EigenGroup
     * @brief %Vector for %Rn, based on the %Eigen library.
     */
    using Vector = Generic::Vector< ::Eigen::VectorXd >;
  }
}

#endif // SPACY_ADAPTER_EIGEN_VECTOR_HH
