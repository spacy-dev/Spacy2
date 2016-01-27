// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_ADAPTER_EIGEN_UTIL_HH
#define SPACY_ADAPTER_EIGEN_UTIL_HH

#include "Eigen/Dense"
#include "Spacy/vector.hh"

namespace Spacy
{
  /** @addtogroup EigenGroup, VectorSpaceGroup @{ */
  namespace Rn
  {
    ///  Copy ::Spacy::Vector to flat coefficient vector of %Eigen .
    void copy(const ::Spacy::Vector& x, Eigen::VectorXd& y);

    ///  Copy flat coefficient vector of %Eigen to ::Spacy::Vector.
    void copy(const Eigen::VectorXd& x, ::Spacy::Vector& y);
    
    unsigned getSize(const ::Spacy::Vector& y);
  }
  /** @} */
}

#endif // SPACY_ADAPTER_EIGEN_UTIL_HH
