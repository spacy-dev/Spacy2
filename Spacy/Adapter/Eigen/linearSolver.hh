// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_ADAPTER_EIGEN_LINEAR_SOLVER_HH
#define SPACY_ADAPTER_EIGEN_LINEAR_SOLVER_HH

#include <utility>

#include <Eigen/Dense>

#include "Spacy/vectorSpace.hh"

namespace Spacy
{
  namespace Rn
  {
    class LinearSolver
    {
    public:
      LinearSolver(::Eigen::MatrixXd A,const VectorSpace& domain);

      Spacy::Vector operator()(const ::Spacy::Vector& y) const;

    private:
      ::Eigen::MatrixXd A_;
      const VectorSpace& domain_;
    };
  }
}

#endif // SPACY_ADAPTER_EIGEN_LINEAR_SOLVER_HH
