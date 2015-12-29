#ifndef SPACY_ADAPTER_EIGEN_C2_FUNCTIONAL_HH
#define SPACY_ADAPTER_EIGEN_C2_FUNCTIONAL_HH

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

    /// A twice differential functional \f$f:X\rightarrow \mathbb{R} \f$ for finite-dimensional problems, based on the %Eigen library.
    class C2Functional : public Spacy::FunctionalBase
    {
    public:
      /**
       * @brief C2Functional
       * @param f callable that specifies the computation of \f$f(x)\f$
       * @param df callable that specifies the computation of \f$f'(x)\f$
       * @param ddf callable that specifies the computation of \f$f''(x)\f$
       * @param domain \f$X\f$
       */
      C2Functional(std::function< double(const ::Eigen::VectorXd&)> f,
                   std::function< ::Eigen::VectorXd(const ::Eigen::VectorXd&) > df,
                   std::function< ::Eigen::MatrixXd(const ::Eigen::VectorXd&) > ddf,
                   const VectorSpace& domain);

      /// Compute f(x).
      Spacy::Real operator()(const ::Spacy::Vector& x) const;

      /// Compute f'(x) as element of X*.
      Spacy::Vector d1(const ::Spacy::Vector& x) const;

      /// Compute f''(x)dx as element of X*.
      Spacy::Vector d2(const ::Spacy::Vector& x, const ::Spacy::Vector& dx) const;

      /// Access f''(x) as mapping f''(x): X->X*.
      LinearOperator hessian(const ::Spacy::Vector& x) const;

    private:
      std::function< double(const ::Eigen::VectorXd&) > f_ ;
      std::function< ::Eigen::VectorXd(const ::Eigen::VectorXd&) > df_;
      std::function< ::Eigen::MatrixXd(const ::Eigen::VectorXd&) > ddf_;
      std::shared_ptr<VectorSpace> operatorSpace_ = nullptr;
    };

    template <class F, class DF, class DDF>
    auto makeC2Functional(const F& f, const DF& df, const DDF& ddf, const Spacy::VectorSpace& domain)
    {
      return C2Functional(f,df,ddf,domain);
    }
  }
  /** @} */
}

#endif // SPACY_ADAPTER_EIGEN_C2_FUNCTIONAL_HH
