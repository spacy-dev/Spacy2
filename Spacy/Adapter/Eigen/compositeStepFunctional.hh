#ifndef SPACY_ADAPTER_EIGEN_COMPOSITE_STEP_FUNCTIONAL_HH
#define SPACY_ADAPTER_EIGEN_COMPOSITE_STEP_FUNCTIONAL_HH

#include <functional>
#include <memory>
#include <Eigen/Dense>

#include "Spacy/Util/Base/functionalBase.hh"


namespace Spacy
{
  /// @cond
  class LinearOperator;
  class Real;
  class Vector;
  /// @endcond

  /** @addtogroup EigenGroup @{ */
  namespace Rn
  {
    /// @cond
    class LinearOperator;
    /// @endcond

    enum class RoleOfFunctional{ NORMAL, TANGENTIAL };

    template<RoleOfFunctional role>
    class CompositeStepFunctional : public Spacy::FunctionalBase
    {
    public:
      /**
       * @brief Constructor for a Lagrange functional.
       * @param f callable that specifies the computation of the cost functional \f$f(x)\f$
       * @param df callable that specifies the computation of the derivative of the cost functional \f$f'(x)\f$
       * @param ddf callable that specifies the computation of the second derivative of the cost functional \f$f''(x)\f$
       * @param c callable that specifies the computation of the constraint \f$c(x)\f$
       * @param dc callable that specifies the computation of the derivative of the constraint \f$c'(x)\f$
       * @param pddc callable that specifies the computation of \f$pc''(x)\f$
       * @param domain \f$X\f$
       */
      CompositeStepFunctional(
          std::function<double(const ::Eigen::VectorXd&)> f,
          std::function< ::Eigen::VectorXd(const ::Eigen::VectorXd&) > df,
          std::function< ::Eigen::MatrixXd(const ::Eigen::VectorXd&) > ddf,
          std::function< ::Eigen::VectorXd(const ::Eigen::VectorXd&) > c,
          std::function< ::Eigen::MatrixXd(const ::Eigen::VectorXd&) > dc,
          std::function< ::Eigen::MatrixXd(const ::Eigen::VectorXd&, const ::Eigen::VectorXd&) > pddc,
          const Spacy::VectorSpace& domain);

      /**
       * @brief Constructor for a Lagrange functional.
       * @param L_T callable that specifies the computation of a Lagrange functional.
       * @param M (local) norm for the computation of the normal step
       */
      CompositeStepFunctional(
          const CompositeStepFunctional<RoleOfFunctional::TANGENTIAL>& L_T,
          std::function< ::Eigen::MatrixXd(const ::Eigen::VectorXd&) > M);

      /// Compute f(x).
      Spacy::Real operator()(const ::Spacy::Vector& x) const;

      /// Compute f'(x) as element of X*.
      Spacy::Vector d1(const ::Spacy::Vector& x) const;

      /// Compute f''(x)dx as element of X*.
      Spacy::Vector d2(const ::Spacy::Vector& x, const ::Spacy::Vector& dx) const;

      /// Access f''(x) as mapping f''(x): X->X*.
      Spacy::Rn::LinearOperator hessian(const ::Spacy::Vector& x) const;

    private:
      std::function<double(const ::Eigen::VectorXd&)> f_;
      std::function< ::Eigen::VectorXd(const ::Eigen::VectorXd&) > df_;
      std::function< ::Eigen::MatrixXd(const ::Eigen::VectorXd&) > ddf_;
      std::function< ::Eigen::VectorXd(const ::Eigen::VectorXd&) > c_;
      std::function< ::Eigen::MatrixXd(const ::Eigen::VectorXd&) > dc_;
      std::function< ::Eigen::MatrixXd(const ::Eigen::VectorXd&, const ::Eigen::VectorXd&) > pddc_;
      std::function< ::Eigen::MatrixXd(const ::Eigen::VectorXd&) > M_;
      std::shared_ptr<VectorSpace> operatorSpace_ = nullptr;

      friend class CompositeStepFunctional<RoleOfFunctional::NORMAL>;
    };

    using NormalStepFunctional = CompositeStepFunctional<RoleOfFunctional::NORMAL>;
    using TangentialStepFunctional = CompositeStepFunctional<RoleOfFunctional::TANGENTIAL>;
  }
  /** @} */
}

#endif // SPACY_ADAPTER_EIGEN_C2_FUNCTIONAL_HH
