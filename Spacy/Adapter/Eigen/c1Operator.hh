// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_ADAPTER_EIGEN_C1OPERATOR_HH
#define SPACY_ADAPTER_EIGEN_C1OPERATOR_HH

#include <functional>
#include <memory>
#include <Eigen/Dense>

#include "Spacy/Util/Base/operatorBase.hh"
#include "Spacy/operator.hh"

namespace Spacy
{
  /// @cond
  class Vector;
  /// @endcond

  namespace Rn
  {
    /**
     * @ingroup EigenGroup
     * @brief A differential operator \f$A:X\rightarrow Y\f$ for finite-dimensional problems, based on the %Eigen library.
     */
    class C1Operator : public Spacy::OperatorBase
    {
    public:
      /**
       * @brief C1Operator
       * @param value callable that specifies the computation of \f$A(x)\f$
       * @param derivative callable that specifies the computation of \f$A'(x)\f$
       * @param domain \f$X\f$
       * @param range \f$Y\f$
       */
      C1Operator(std::function< ::Eigen::VectorXd(const ::Eigen::VectorXd&) > value,
                 std::function< ::Eigen::MatrixXd(const ::Eigen::VectorXd&) > derivative,
                 const VectorSpace& domain,
                 const VectorSpace& range);

      /// Compute A(x).
      ::Spacy::Vector operator()(const ::Spacy::Vector& x) const;

      /// Compute A'(x)dx.
      ::Spacy::Vector d1(const ::Spacy::Vector& x, const ::Spacy::Vector& dx) const;

      /// Get linearization representing A'(x).
      ::Spacy::LinearOperator linearization(const ::Spacy::Vector& x) const;

    private:
      std::function< ::Eigen::VectorXd(const ::Eigen::VectorXd&) > value_;
      std::function< ::Eigen::MatrixXd(const ::Eigen::VectorXd&) > derivative_;
      std::shared_ptr<VectorSpace> operatorSpace_;
    };
  }
}

#endif // SPACY_ADAPTER_EIGEN_C1OPERATOR_HH
