// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_ADAPTER_EIGEN_VECTOR_CREATOR_HH
#define SPACY_ADAPTER_EIGEN_VECTOR_CREATOR_HH

#include <Eigen/Dense>

#include "Spacy/Adapter/Generic/vectorCreator.hh"

namespace Spacy
{
  namespace Rn
  {
    /**
     * @ingroup EigenGroup
     * @brief %Vector creator for %Rn, based on the %Eigen library.
     */
    using VectorCreator = Generic::VectorCreator< ::Eigen::VectorXd >;

    /**
     * @ingroup EigenGroup
     * @brief Convenient generation of a hilbert space based on the %Eigen library.
     * @return @ref ::Spacy::makeHilbertSpace() "::Spacy::makeHilbertSpace( VectorCreator{dim} , EuclideanScalarProduct{} )"
     */
    VectorSpace makeHilbertSpace(unsigned dim);
  }
}

#endif // SPACY_ADAPTER_EIGEN_VECTOR_CREATOR_HH

