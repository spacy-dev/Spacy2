// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_ADAPTER_EIGEN_LINEAR_OPERATOR_HH
#define SPACY_ADAPTER_EIGEN_LINEAR_OPERATOR_HH

#include <Eigen/Dense>

#include "Spacy/Util/Mixins/impl.hh"
#include "Spacy/Util/Base/addArithmeticOperators.hh"
#include "Spacy/Util/Base/operatorBase.hh"
#include "Spacy/Util/Base/vectorBase.hh"
#include "Spacy/linearSolver.hh"

namespace Spacy
{
  namespace Rn
  {
    /**
     * @ingroup EigenGroup
     * @brief A linear operator \f$A:X\rightarrow Y\f$ for finite-dimensional problems, based on the %Eigen library.
     */
    class LinearOperator :
      public Mixin::Impl< ::Eigen::MatrixXd >,
      public VectorBase,
      public OperatorBase ,
      public AddArithmeticOperators<LinearOperator>
    {
    public:
      /**
       * @brief LinearOperator
       * @param A matrix specifying the linear operator
       * @param space space of linear operators \f$X\rightarrow Y\f$
       * @param domain \f$X\f$
       * @param range \f$Y\f$
       */
      LinearOperator(::Eigen::MatrixXd A, const VectorSpace& space, const VectorSpace& domain, const VectorSpace& range);

      /// Compute \f$A(x)\f$
      ::Spacy::Vector operator()(const ::Spacy::Vector& dx) const;

      /// Access solver representing \f$A^{-1}\f$
      ::Spacy::LinearSolver solver() const;
    };
  }
}

#endif // SPACY_ADAPTER_EIGEN_LINEAR_OPERATOR_HH
