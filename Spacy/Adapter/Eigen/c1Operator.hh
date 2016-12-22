#pragma once

#include <functional>
#include <memory>

#include <Spacy/Util/Base/OperatorBase.hh>

#include <Eigen/Dense>

namespace Spacy
{
  /// @cond
  class Vector;
  /// @endcond

  namespace Rn
  {
    /// @cond
    class LinearOperator;
    /// @endcond

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
      LinearOperator linearization(const ::Spacy::Vector& x) const;

    private:
      std::function< ::Eigen::VectorXd(const ::Eigen::VectorXd&) > value_;
      std::function< ::Eigen::MatrixXd(const ::Eigen::VectorXd&) > derivative_;
      std::shared_ptr<VectorSpace> operatorSpace_;
    };

    template <class FunGOperator, int variable_id = 0>
    auto getFunGC1Operator(FunGOperator& A, const VectorSpace& domain, const VectorSpace& range)
    {
        auto value = [&A](const ::Eigen::VectorXd& x) -> ::Eigen::VectorXd
        {
            A.template update<variable_id>(x);
            return A();
        };

        auto derivative = [&A](const ::Eigen::VectorXd& x) -> Eigen::MatrixXd
        {
            A.template update<variable_id>(x);

            ::Eigen::MatrixXd M(x.rows(),x.rows());
            M.fill(0);
            auto y = x;
            for(auto i=0; i<x.rows(); ++i)
            {
                y.fill(0);
                y(i) = 1;
                auto z = A.template d1<variable_id>(y);
                for(auto j=0; j<x.rows(); ++j)
                    M(j,i) = z(j);
            }
            return M;
        };

        return C1Operator( std::move(value), std::move(derivative), domain, range );
    }
  }
}
