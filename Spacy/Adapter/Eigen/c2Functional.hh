#pragma once

#include <functional>
#include <memory>
#include <Eigen/Dense>

#include <Spacy/Util/Base/FunctionalBase.hh>

namespace Spacy
{
    /// @cond
    class LinearOperator;
    class Real;
    class Vector;
    /// @endcond

    /** @addtogroup ScalarGroup @{ */
    namespace Rn
    {
        /// @cond
        class LinearOperator;
        /// @endcond

        class C2Functional : public Spacy::FunctionalBase
        {
        public:
            C2Functional(std::function<double(const ::Eigen::VectorXd&)> f,
                         std::function< ::Eigen::VectorXd(const ::Eigen::VectorXd&) > df,
                         std::function< ::Eigen::MatrixXd(const ::Eigen::VectorXd&) > ddf,
                         const VectorSpace& domain);

            // Compute f(x).
            Spacy::Real operator()(const ::Spacy::Vector& x) const;

            // Compute f'(x) as element of X*.
            Spacy::Vector d1(const ::Spacy::Vector& x) const;

            // Compute f''(x)dx as element of X*.
            Spacy::Vector d2(const ::Spacy::Vector& x, const ::Spacy::Vector& dx) const;

            // Access f''(x) as mapping f''(x): X->X*.
            LinearOperator hessian(const ::Spacy::Vector& x) const;

        private:
            std::shared_ptr<VectorSpace> operatorSpace_ = nullptr;
            std::function<double(const ::Eigen::VectorXd&)> f_ ;
            std::function<::Eigen::VectorXd(const ::Eigen::VectorXd&)> df_;
            std::function<::Eigen::MatrixXd(const ::Eigen::VectorXd&)> ddf_;
        };

        template <class F, class DF, class DDF>
        auto makeC2Functional(const F& f, const DF& df, const DDF& ddf, const Spacy::VectorSpace& domain)
        {
            return C2Functional(f,df,ddf,domain);
        }
    }
    /** @} */
}
