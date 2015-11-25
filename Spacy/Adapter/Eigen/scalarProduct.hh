// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_ADAPTER_EIGEN_SCALAR_PRODUCT_HH
#define SPACY_ADAPTER_EIGEN_SCALAR_PRODUCT_HH

#include <Eigen/Dense>

#include "Spacy/Adapter/Generic/scalarProduct.hh"

namespace Spacy
{
  namespace Rn
  {
    /**
     * @ingroup EigenGroup
     * @brief Euclidean scalar product for %Rn, based on the %Eigen library.
     */
    using EuclideanScalarProduct = Generic::EuclideanScalarProduct;
  }
}


#endif // SPACY_ADAPTER_EIGEN_SCALAR_PRODUCT_HH

